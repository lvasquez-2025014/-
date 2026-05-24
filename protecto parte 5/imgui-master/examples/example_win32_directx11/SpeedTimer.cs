using System;
using System.Threading;
using System.Threading.Tasks;

namespace AotForms
{
    /// <summary>
    /// SpeedTimer with Anti-Detection techniques
    /// Uses randomization and variable delays to avoid anti-cheat detection
    /// </summary>
    internal static class SpeedTimer
    {
        private static Task speedTask;
        private static CancellationTokenSource cts;
        private static bool isRunning = false;
        private static Random random = new Random();
        
        // Default timer value (normal speed)
        private const float NormalSpeed = 0.033000f;
        
        // Speed multiplier values
        private static float currentSpeedValue = NormalSpeed;
        
        // Speed level (0 = off, 2 = x2, 5 = x5, 10 = x10, etc)
        public static int SpeedMultiplier { get; set; } = 0;

        public static bool IsEnabled => SpeedMultiplier > 0;

        // Anti-detection settings
        private static int applyCounter = 0;
        private const int ApplyEveryNFrames = 3; // Apply every 3rd frame to reduce detection
        private const float SpeedVariation = 0.005f; // Small random variation

        internal static void Start(int multiplier)
        {
            SpeedMultiplier = multiplier;
            currentSpeedValue = NormalSpeed * multiplier;
            applyCounter = 0;
            
            if (isRunning) return;
            isRunning = true;

            cts = new CancellationTokenSource();
            speedTask = Task.Run(() => LoopAsync(cts.Token), cts.Token);
            Console.WriteLine($"[SpeedTimer] Started at x{multiplier} (Anti-Detect Mode)");
        }

        internal static void Stop()
        {
            if (!isRunning) return;
            
            SpeedMultiplier = 0;
            
            // Reset to normal speed before stopping
            ResetSpeed();
            
            cts?.Cancel();
            try { speedTask?.Wait(500); } catch { }
            isRunning = false;
            Console.WriteLine("[SpeedTimer] Stopped");
        }

        private static async Task LoopAsync(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                try
                {
                    if (!IsEnabled)
                    {
                        await Task.Delay(50, token);
                        continue;
                    }

                    // Anti-detection: Apply only every N frames
                    applyCounter++;
                    if (applyCounter >= ApplyEveryNFrames)
                    {
                        ApplySpeedHackSafe();
                        applyCounter = 0;
                    }
                }
                catch (OperationCanceledException)
                {
                    break;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[SpeedTimer] Error: {ex.Message}");
                }

                // Anti-detection: Variable delay (15-25ms instead of fixed 10ms)
                int delay = 15 + random.Next(0, 11);
                await Task.Delay(delay, token);
            }
        }

        private static void ApplySpeedHackSafe()
        {
            try
            {
                // Read currentGame from static
                var rBaseGameFacade = InternalMemory.Read<uint>(Offsets.Il2Cpp + Offsets.InitBase, out var baseGameFacade);
                if (!rBaseGameFacade || baseGameFacade == 0) return;

                var rGameFacade = InternalMemory.Read<uint>(baseGameFacade, out var gameFacade);
                if (!rGameFacade || gameFacade == 0) return;

                var rStaticGameFacade = InternalMemory.Read<uint>(gameFacade + Offsets.StaticClass, out var staticGameFacade);
                if (!rStaticGameFacade || staticGameFacade == 0) return;

                var rCurrentGame = InternalMemory.Read<uint>(staticGameFacade, out var currentGame);
                if (!rCurrentGame || currentGame == 0) return;

                // Read timer pointer
                var rTimerPtr = InternalMemory.Read<uint>(currentGame + Offsets.TimerPtr, out var speedTimer);
                if (!rTimerPtr || speedTimer == 0) return;

                // Anti-detection: Add small random variation to speed value
                float variation = ((float)random.NextDouble() - 0.5f) * SpeedVariation;
                float safeSpeedValue = currentSpeedValue + variation;
                
                // Write speed value with variation
                InternalMemory.Write(speedTimer + Offsets.Timer, safeSpeedValue);
            }
            catch { }
        }

        private static void ResetSpeed()
        {
            try
            {
                var rBaseGameFacade = InternalMemory.Read<uint>(Offsets.Il2Cpp + Offsets.InitBase, out var baseGameFacade);
                if (!rBaseGameFacade || baseGameFacade == 0) return;

                var rGameFacade = InternalMemory.Read<uint>(baseGameFacade, out var gameFacade);
                if (!rGameFacade || gameFacade == 0) return;

                var rStaticGameFacade = InternalMemory.Read<uint>(gameFacade + Offsets.StaticClass, out var staticGameFacade);
                if (!rStaticGameFacade || staticGameFacade == 0) return;

                var rCurrentGame = InternalMemory.Read<uint>(staticGameFacade, out var currentGame);
                if (!rCurrentGame || currentGame == 0) return;

                var rTimerPtr = InternalMemory.Read<uint>(currentGame + Offsets.TimerPtr, out var speedTimer);
                if (!rTimerPtr || speedTimer == 0) return;

                // Reset to normal speed
                InternalMemory.Write(speedTimer + Offsets.Timer, NormalSpeed);
            }
            catch { }
        }
    }
}
