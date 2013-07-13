using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;


namespace mremGUI
{
    public partial class Form1 : Form
    {
    [DllImport("user32.dll")] public static extern IntPtr PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

    [DllImport("user32.dll", SetLastError = true)] public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    const int WM_USER = 0x0400;

        public Form1()
        {
            InitializeComponent();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            MessageBox.Show(this, "I intially wrote all the code for keyboard hooking and the registry trick to open the Start screen only when Start button is pressed on the keyboard in C++ but was having few issues with MFC so porting the code to C# will take sometime. Please wait a bit for this! Will be pushing an update asap", "Message");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Process.Start("mrem.exe", "--killstart");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Process.Start("mrem.exe", "--killmetro");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string pthexplorer = string.Format("{0}\\{1}", Environment.GetEnvironmentVariable("WINDIR"), "explorer.exe");
            IntPtr shell_TW;
            shell_TW = FindWindow("Shell_TrayWnd", "");
            PostMessage(shell_TW, WM_USER + 436, IntPtr.Zero, IntPtr.Zero);
            Process process = new Process();
            process.StartInfo.FileName = pthexplorer;
            process.StartInfo.UseShellExecute = true;
            process.Start();

        }
        private void button5_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
