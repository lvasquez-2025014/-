using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Diagnostics;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace AotForms
{
    internal static class Offsets
    {
        internal static uint Il2Cpp;
    }

    internal class Adb
    {
        string _path;
        StringBuilder _builder;
        Process _process;
        ConcurrentQueue<string> _outputQueue;

        internal Adb(string path)
        {
            _path = path;
            _builder = new StringBuilder();
            _outputQueue = new ConcurrentQueue<string>();
        }

        internal async Task Kill()
        {
            await Task.Run(() => {
                ExecuteAdbCommand("kill-server");

                var adbProcesses = Process.GetProcessesByName("HD-Adb");
                foreach (var adbProcess in adbProcesses)
                {
                    try
                    {
                        adbProcess.Kill();
                        adbProcess.WaitForExit();
                    }
                    catch { }
                }
                return Task.CompletedTask;
            });
        }

        internal async Task<bool> Start()
        {
            return await Task.Run(() => {
                ExecuteAdbCommand("kill-server");
                ExecuteAdbCommand("start-server");

                // Dynamic port resolution
                int port = 5555;
                if (BsAdbResolver.InitAdbPort() && BsAdbResolver.AdbPort > 0)
                {
                    port = BsAdbResolver.AdbPort;
                }

                ExecuteAdbCommand($"connect 127.0.0.1:{port}");
                ExecuteAdbCommand("devices");

                _process = new Process();
                _process.StartInfo.FileName = _path;
                _process.StartInfo.Arguments = $"-s 127.0.0.1:{port} shell \"getprop ro.secure ; /boot/android/android/system/xbin/bstk/su\"";
                _process.StartInfo.RedirectStandardOutput = true;
                _process.StartInfo.RedirectStandardError = true;
                _process.StartInfo.RedirectStandardInput = true;
                _process.StartInfo.UseShellExecute = false;
                _process.StartInfo.CreateNoWindow = true;
                _process.EnableRaisingEvents = true;
                _process.OutputDataReceived += Receiver;
                _process.ErrorDataReceived += Receiver;

                _process.Start();
                _process.BeginOutputReadLine();
                _process.BeginErrorReadLine();

                _process.StandardInput.AutoFlush = true;

                // Wait up to 2.5 seconds for initial shell warm-up/output
                int timeout = 2500;
                while (_builder.Length == 0 && timeout > 0)
                {
                    Thread.Sleep(5);
                    timeout -= 5;
                }

                return true;
            });
        }

        void Receiver(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                _builder.AppendLine(e.Data);
                _outputQueue.Enqueue(e.Data);
            }
        }

        async Task<string> WaitForOutput(int timeoutMs = 5000)
        {
            string output = null;
            var sw = Stopwatch.StartNew();
            while (sw.ElapsedMilliseconds < timeoutMs)
            {
                if (_outputQueue.TryDequeue(out output))
                    return output;
                await Task.Delay(1);
            }
            return null;
        }

        async Task<string> WaitForOutputAndExtractPID(string processName, int timeoutMs = 5000)
        {
            var sw = Stopwatch.StartNew();
            while (sw.ElapsedMilliseconds < timeoutMs)
            {
                string line = await WaitForOutput(timeoutMs - (int)sw.ElapsedMilliseconds);
                if (line == null) break;
                if (line.Contains(processName))
                {
                    var parts = line.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    if (parts.Length > 1)
                        return parts[1]; // Typically PID is the second column in ps output
                }
            }
            return null;
        }

        internal async Task<uint> FindModule(string process, string module)
        {
            return await Task.Run(async () => {
                _builder.Clear();
                _outputQueue = new ConcurrentQueue<string>();

                _process.StandardInput.WriteLine("ps");

                string pid = await WaitForOutputAndExtractPID(process);
                if (string.IsNullOrEmpty(pid)) return 0U;

                _builder.Clear();
                _outputQueue = new ConcurrentQueue<string>();
                _process.StandardInput.WriteLine($"cat proc/{pid}/maps | grep {module}");

                string mapsLine = await WaitForOutput();
                if (string.IsNullOrEmpty(mapsLine)) return 0U;

                _process.Kill();
                _process.WaitForExit();
                await Kill();

                var mod = mapsLine.Split('-');
                return Convert.ToUInt32(mod[0], 16);
            });
        }

        internal async Task<uint> OXBYTES(string process, string module)
        {
            return await Task.Run(async () =>
            {
                _outputQueue = new ConcurrentQueue<string>();

                _process.StandardInput.WriteLine("ps");

                string pid = await WaitForOutputAndExtractPID(process);
                if (string.IsNullOrEmpty(pid)) return 0U;

                _outputQueue = new ConcurrentQueue<string>();
                _process.StandardInput.WriteLine($"cat proc/{pid}/maps | grep {module} | sed -n '2p'");

                string address = await WaitForOutput();
                if (string.IsNullOrEmpty(address)) return 0U;

                _process.Kill();
                _process.WaitForExit();
                await Kill();

                uint BaseAdress = Convert.ToUInt32(address.Split('-')[0], 16);
                Offsets.Il2Cpp = BaseAdress;
                return BaseAdress;
            });
        }

        void ExecuteAdbCommand(string command)
        {
            try
            {
                var proc = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = _path,
                        Arguments = command,
                        UseShellExecute = false,
                        RedirectStandardOutput = false,
                        RedirectStandardError = false,
                        CreateNoWindow = true
                    }
                };
                proc.Start();
                proc.WaitForExit();
            }
            catch { }
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

            string configPath = null;
            if (exePath.Contains("bluestacks_msi5"))
                configPath = MSI5_CONFIG;
            else if (exePath.Contains("bluestacks_nxt"))
                configPath = BS_CONFIG;

            if (configPath == null || !File.Exists(configPath))
            {
                if (File.Exists(BS_CONFIG))
                    configPath = BS_CONFIG;
                else if (File.Exists(MSI5_CONFIG))
                    configPath = MSI5_CONFIG;
            }

            if (configPath == null || !File.Exists(configPath))
                return false;

            return ResolvePortFromConfig(configPath, windowTitle);
        }

        private static bool ResolvePortFromConfig(string configPath, string displayName)
        {
            try
            {
                string[] lines = File.ReadAllLines(configPath);
                string instanceKey = null;

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
            }
            catch { }

            return false;
        }
    }
}