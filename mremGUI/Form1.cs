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

        private void button3_Click(object sender, EventArgs e)
        {
            Process.Start("mrem.exe", "--sbopenav"); 
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
            Process.Start("mrem.exe", "--resexplr");

        }
        private void button5_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
