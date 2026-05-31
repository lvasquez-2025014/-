using System;
using System.Numerics;
using System.Threading;
using System.Threading.Tasks;

namespace AotForms
{
    internal class Speed
    {
        private static Task teleportTask;
        private static CancellationTokenSource cts;
        private static bool isRunning = false;
        private static Random random = new Random();

        public static void StartForwardTeleport()
        {
            if (isRunning) return;

            cts = new CancellationTokenSource();
            isRunning = true;

            teleportTask = Task.Run(async () =>
            {
                // Anti-detection: Random initial delay
                await Task.Delay(random.Next(100, 500));
                
                while (!cts.Token.IsCancellationRequested)
                {
                    try
                    {
                        if (Config.EnableForwardTeleport && Core.LocalPlayer != 0 && Core.HaveMatrix)
                        {
                            // Anti-detection: Random skip
                            if (random.Next(0, 100) < 8)
                            {
                                await Task.Delay(random.Next(50, 150), cts.Token);
                                continue;
                            }
                            
                            // Random distance variation
                            float distance = Config.TeleSpeed / 100f;
                            if (random.Next(0, 100) < 30)
                            {
                                distance *= (0.7f + (float)random.NextDouble() * 0.6f);
                            }
                            
                            TeleportForward(distance);
                        }
                        
                        // Anti-detection: Variable delay
                        int delay = random.Next(Config.thread > 0 ? Config.thread : 50, 
                                                (Config.thread > 0 ? Config.thread : 50) + 30);
                        await Task.Delay(delay, cts.Token);
                    }
                    catch
                    {
                        await Task.Delay(random.Next(50, 150), cts.Token);
                    }
                }
            }, cts.Token);
        }

        public static void StopForwardTeleport()
        {
            if (!isRunning) return;

            cts?.Cancel();
            isRunning = false;
        }

        private static void TeleportForward(float distance)
        {
            try
            {
                // Updated: Using Bones.Root
                var rootResult = InternalMemory.Read<uint>(Core.LocalPlayer + (uint)Bones.Root, out uint localRootBonePtr);
                if (!rootResult || localRootBonePtr == 0) return;

                // Updated: Using Transform.GetNodePosition
                if (!Transform.GetNodePosition(localRootBonePtr, out Vector3 currentPosition)) return;

                // Get player rotation for forward direction
                var rotationRead = InternalMemory.Read<Quaternion>(Core.LocalPlayer + Offsets.AimRotation, out var playerRotation);
                if (!rotationRead) return;

                // Calculate forward direction
                Vector3 forward = GetForwardDirection(playerRotation);
                
                // Normalize forward vector and keep Y as is (don't fly)
                forward = Vector3.Normalize(forward);
                forward.Y = 0;

                // Calculate new position
                Vector3 newPosition = currentPosition + (forward * distance);

                // Anti-detection: Add small random jitter
                if (random.Next(0, 100) < 25)
                {
                    newPosition.X += (float)(random.NextDouble() - 0.5) * 0.1f;
                    newPosition.Z += (float)(random.NextDouble() - 0.5) * 0.1f;
                }

                // Anti-detection: Small random delay before write
                if (random.Next(0, 100) < 10)
                    Thread.Sleep(random.Next(1, 3));

                // Updated: Using Transform.SetNodePosition
                Transform.SetNodePosition(localRootBonePtr, newPosition);
            }
            catch { }
        }

        private static Vector3 GetForwardDirection(Quaternion rotation)
        {
            float x = rotation.X;
            float y = rotation.Y;
            float z = rotation.Z;
            float w = rotation.W;

            // Calculate forward vector (0, 0, 1) rotated by quaternion
            float fx = 2 * (x * z + w * y);
            float fy = 2 * (y * z - w * x);
            float fz = 1 - 2 * (x * x + y * y);

            return new Vector3(fx, fy, fz);
        }
    }
}