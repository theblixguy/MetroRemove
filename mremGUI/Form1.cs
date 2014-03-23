using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;


namespace mremGUI
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void Startmrem(string args)
        {
            Process process = new Process();
            process.StartInfo.FileName = Application.StartupPath + @"\mrem.exe";
            process.StartInfo.Arguments = args;
            process.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            process.Start();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Startmrem("--sbopenav");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Startmrem("--killstart");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Startmrem("--killmetro");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Startmrem("--resexplr");
        }
        private void button5_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
