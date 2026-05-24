using System;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace AotForms
{
    public static class AdbHelper
    {
        static Adb instance;
        public static string GetEmulatorAdbFolder()
        {

            string[] emulatorProcesses = { "HD-Player", "Bluestacks", "DnPlayer", "Nox", "Memu", "MuMuPlayer", "LDPlayer" };

            foreach (var name in emulatorProcesses)
            {
                var proc = Process.GetProcessesByName(name).FirstOrDefault();
                if (proc != null && !proc.HasExited)
                {
                    try
                    {
                        string path = proc.MainModule?.FileName;
                        if (!string.IsNullOrEmpty(path))
                            return Path.GetDirectoryName(path);
                    }
                    catch { }
                }
            }

            return null;
        }

        public static async void Initilize()
        {
            BsAdbResolver.InitAdbPort();
            try
            {
                string adbFolder = GetEmulatorAdbFolder();
                if (string.IsNullOrEmpty(adbFolder))
                {
                    MessageBox.Show("No supported emulator (HD-Player, LDPlayer, Nox, etc.) is running.\nPlease start the emulator first.",
                        "Emulator not found", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }

                string adbPath = Path.Combine(adbFolder, "adb.exe");
                if (!File.Exists(adbPath))
                {
                    adbPath = Path.Combine(adbFolder, "HD-Adb.exe"); // fallback for BlueStacks
                    if (!File.Exists(adbPath))
                    {
                        MessageBox.Show($"ADB not found in:\n{adbFolder}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }
                // MessageBox.Show($"{BsAdbResolver.AdbPort}");
                instance = new Adb(adbPath, BsAdbResolver.AdbPort);
                await instance.AdbConnect();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Initialization failed:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public static async Task<ulong> ModuleAddrAsync(string package, string module)
        {
            return await instance.ModuleAddress(package, module);
        }

        public static async Task<string> GetFreeFirePKG()
        {
            return await instance.GetRunningFreeFirePackage();
        }

        public static async void Close()
        {
            await instance.KillAdb();
        }
    }
    internal class Adb
    {
        static string _adbpath;
        static string _adbTarget = "";
        static Process _process;
        static StringBuilder _builder;
        static int _port = 5555;

        public Adb(string adbpath, int port = 5555)
        {
            _adbpath = adbpath;
            _builder = new StringBuilder();
            _process = null;
            _port = port;
            _adbTarget = $"-s 127.0.0.1:{port}";
        }

        internal async Task KillAdb()
        {
            await Task.Run(() =>
            {
                try
                {
                    foreach (var p in Process.GetProcessesByName("Adb"))
                    {
                        p.Kill();
                        p.WaitForExit();
                    }
                    foreach (var p in Process.GetProcessesByName("HD-Adb"))
                    {
                        p.Kill();
                        p.WaitForExit();
                    }
                }
                catch { }

                return Task.CompletedTask;
            });
        }

        internal async Task<bool> AdbConnect()
        {
            return await Task.Run(() =>
            {
                ExecuteAdbCmd("kill-server");
                ExecuteAdbCmd("start-server");
                ExecuteAdbCmd($"connect 127.0.0.1:{_port}");

                _builder.Clear();
                _process = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = _adbpath,
                        Arguments = $"{_adbTarget} shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\"",
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        RedirectStandardError = true,
                        RedirectStandardInput = true,
                        CreateNoWindow = true
                    },
                    EnableRaisingEvents = true
                };

                var tcs = new TaskCompletionSource<bool>();

                DataReceivedEventHandler handler = (s, e) =>
                {
                    if (e.Data != null)
                    {
                        _builder.AppendLine(e.Data);
                        tcs.TrySetResult(true);
                    }
                };

                _process.OutputDataReceived += handler;
                _process.ErrorDataReceived += handler;

                try
                {
                    _process.Start();
                    _process.BeginOutputReadLine();
                    _process.BeginErrorReadLine();

                    return tcs.Task.Wait(1500) && !_process.HasExited;
                }
                finally
                {
                    _process.OutputDataReceived -= handler;
                    _process.ErrorDataReceived -= handler;
                }
            });
        }

        private async Task WarmUpShell()
        {
            await Task.Run(() =>
            {
                try
                {
                    var tcs = new TaskCompletionSource<bool>();

                    DataReceivedEventHandler h = (s, e) =>
                    {
                        if (e.Data != null)
                            tcs.TrySetResult(true);
                    };

                    _process.OutputDataReceived += h;
                    _process.StandardInput.WriteLine("echo .");
                    _process.StandardInput.Flush();
                    tcs.Task.Wait(600);
                    _process.OutputDataReceived -= h;
                }
                catch { }
            });
        }

        private async Task<bool> EnsureShellIsReady()
        {
            if (_process != null && !_process.HasExited)
            {
                await WarmUpShell();
                return true;
            }

            bool ok = await AdbConnect();
            if (ok) await WarmUpShell();
            return ok;
        }

        public static void ExecuteAdbCmd(string cmd)
        {
            var p = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = _adbpath,
                    Arguments = $"{_adbTarget} {cmd}",
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    CreateNoWindow = true
                }
            };
            p.Start();
            p.WaitForExit();
        }


        internal async Task<uint> ModuleAddress(string packagename, string module)
        {
            return await Task.Run(() =>
            {
                _builder.Clear();

                var pidTcs = new TaskCompletionSource<string>();
                DataReceivedEventHandler pidHandler = (s, e) =>
                {
                    if (e.Data != null && int.TryParse(e.Data.Trim(), out _))
                        pidTcs.TrySetResult(e.Data.Trim());
                };

                _process.OutputDataReceived += pidHandler;

                try
                {
                    _process.StandardInput.WriteLine($"pidof {packagename}");
                    _process.StandardInput.Flush();

                    string pid = pidTcs.Task.Wait(800) ? pidTcs.Task.Result : null;
                    if (string.IsNullOrEmpty(pid))
                        return 1U;

                    _process.OutputDataReceived -= pidHandler;
                    _builder.Clear();

                    var addrTcs = new TaskCompletionSource<string>();
                    DataReceivedEventHandler addrHandler = (s, e) =>
                    {
                        if (e.Data != null && e.Data.Contains(module) && e.Data.Contains("-"))
                        {
                            var start = e.Data.Split('-')[0].Trim();
                            if (start.Length >= 8 && start.All(c => "0123456789abcdefABCDEF".Contains(c)))
                                addrTcs.TrySetResult(start);
                        }
                    };

                    _process.OutputDataReceived += addrHandler;
                    _process.StandardInput.WriteLine($"grep -m 1 '{module}' /proc/{pid}/maps");
                    _process.StandardInput.Flush();

                    string addrLine = addrTcs.Task.Wait(800) ? addrTcs.Task.Result : null;
                    if (string.IsNullOrEmpty(addrLine))
                        return 1U;

                    return uint.Parse(addrLine, System.Globalization.NumberStyles.HexNumber);
                }
                finally
                {
                    _process.OutputDataReceived -= pidHandler;
                }
            });
        }

        public async Task<string> GetRunningFreeFirePackage()
        {
            if (!await EnsureShellIsReady())
                return string.Empty;

            return await Task.Run(() =>
            {
                _builder.Clear();
                var tcs = new TaskCompletionSource<string>();

                DataReceivedEventHandler handler = (s, e) =>
                {
                    if (e.Data == null) return;

                    if (e.Data.Contains("mResumedActivity") || e.Data.Contains("topResumedActivity"))
                    {
                        int idx = e.Data.IndexOf("u0 ");
                        if (idx >= 0)
                        {
                            string part = e.Data.Substring(idx + 3);
                            int slash = part.IndexOf('/');
                            if (slash > 0)
                            {
                                string pkg = part.Substring(0, slash).Trim();
                                if (pkg == "com.dts.freefireth" || pkg == "com.dts.freefiremax")
                                    tcs.TrySetResult(pkg);
                            }
                        }
                    }
                };

                try
                {
                    _process.OutputDataReceived += handler;
                    _process.StandardInput.WriteLine(
                        "dumpsys activity activities | grep -m 1 -E 'mResumedActivity|topResumedActivity'");
                    _process.StandardInput.Flush();

                    if (tcs.Task.Wait(1200))
                        return tcs.Task.Result;

                    _builder.Clear();
                    _process.StandardInput.WriteLine("pidof com.dts.freefiremax");
                    _process.StandardInput.Flush();
                    if (!string.IsNullOrEmpty(_builder.ToString().Trim()))
                        return "com.dts.freefiremax";

                    _builder.Clear();
                    _process.StandardInput.WriteLine("pidof com.dts.freefireth");
                    _process.StandardInput.Flush();
                    if (!string.IsNullOrEmpty(_builder.ToString().Trim()))
                        return "com.dts.freefireth";

                    return string.Empty;
                }
                finally
                {
                    _process.OutputDataReceived -= handler;
                }
            });
        }
    

    }
    internal static class BsAdbResolver
    {
        public static int AdbPort;

        private const string BS_CONFIG = @"C:\ProgramData\BlueStacks_nxt\bluestacks.conf";
        private const string MSI5_CONFIG = @"C:\ProgramData\BlueStacks_msi5\bluestacks.conf";

        public static bool InitAdbPort()
        {
            var hd = Process.GetProcessesByName("HD-Player").FirstOrDefault();
            if (hd == null)
                return false;

            string windowTitle = hd.MainWindowTitle;
            if (string.IsNullOrEmpty(windowTitle))
                return false;

            string exePath;
            try
            {
                exePath = hd.MainModule.FileName.ToLower();
            }
            catch
            {
                return false;
            }

            // 🔥 EXACT DETECTION (NO GUESSING)
            string configPath = null;

            if (exePath.Contains("bluestacks_msi5"))
                configPath = MSI5_CONFIG;
            else if (exePath.Contains("bluestacks_nxt"))
                configPath = BS_CONFIG;

            if (configPath == null || !File.Exists(configPath))
                return false;

            return ResolvePortFromConfig(configPath, windowTitle);
        }

        private static bool ResolvePortFromConfig(string configPath, string displayName)
        {
            string[] lines = File.ReadAllLines(configPath);
            string instanceKey = null;

            // 1️⃣ Match instance
            foreach (string line in lines)
            {
                if (line.Contains("display_name") && line.Contains($"\"{displayName}\""))
                {
                    int idx = line.IndexOf(".display_name", StringComparison.Ordinal);
                    if (idx > 0)
                    {
                        instanceKey = line.Substring(0, idx);
                        break;
                    }
                }
            }

            if (instanceKey == null)
                return false;

            // 2️⃣ Extract adb port
            foreach (string line in lines)
            {
                if (line.StartsWith(instanceKey) && line.Contains("status.adb_port"))
                {
                    var m = Regex.Match(line, "\"(\\d+)\"");
                    if (m.Success)
                    {
                        AdbPort = int.Parse(m.Groups[1].Value);
                        return true;
                    }
                }
            }

            return false;
        }

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern int GetWindowText(IntPtr hWnd, StringBuilder text, int count);
    }

}