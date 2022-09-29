using System;
using System.Runtime.InteropServices;

namespace U
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Quaternion(float w, Vector3 xyz)
        {
            X = xyz.X;
            Y = xyz.Y;
            Z = xyz.Z;
            W = w;

        }

        public Quaternion(Vector3 eulerAngles)
        {
            Vector3 c = Vector3.Cos(eulerAngles * 0.5F);
            Vector3 s = Vector3.Sin(eulerAngles * 0.5F);

            W = c.X * c.Y * c.Z + s.X * s.Y * s.Z;
            X = s.X * c.Y * c.Z - c.X * s.Y * s.Z;
            Y = c.X * s.Y * c.Z + s.X * c.Y * s.Z;
            Z = c.X * c.Y * s.Z - s.X * s.Y * c.Z;
        }

        public static Quaternion AngleAxis(float angle, Vector3 axis)
        {
            float s = (float)Math.Sin(angle * 0.5F);
            return new Quaternion(s, axis * s);
        }

        public static Quaternion operator *(Quaternion a, Quaternion b)
        {
            float w = a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z;
            float x = a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y;
            float y = a.W * b.Y + a.Y * b.W + a.Z * b.X - a.X * b.Z;
            float z = a.W * b.Z + a.Z * b.W + a.X * b.Y - a.Y * b.X;
            return new Quaternion(x, y, z, w);
        }

    }
}
