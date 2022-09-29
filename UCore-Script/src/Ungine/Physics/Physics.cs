using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;


namespace U
{
    [StructLayout(LayoutKind.Sequential)]
    public class RaycastHit
    {
        public ulong EntityID { get; private set; }
        public Vector3 Position { get; private set; }
        public Vector3 Normal { get; private set; }
        public float Distance { get; private set; }

    }

    public static class Physics
    {
        public static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, out RaycastHit hit)
        {
            return Raycast_Native(ref origin, ref direction, maxDistance, out hit);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Raycast_Native(ref Vector3 origin, ref Vector3 direction, float maxDistance, out RaycastHit hit);

    }
}
