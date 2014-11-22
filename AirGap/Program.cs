using AirGap.Receive;
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using AirGap.Signal;

namespace AirGap
{
    static class Program
    {
        /// <summary>
        /// Der Haupteinstiegspunkt für die Anwendung.
        /// </summary>
        [STAThread]
        static void Main()
        {
            AppDomain.CurrentDomain.AssemblyResolve += CurrentDomain_AssemblyResolve;

            var connector = new Connector();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //Empfangen:
            // Mikrofon -> Filter -> Make Complex -> divide -> quadrature_demod_cf ->
            //    clock_recovery_mm_ff -> binary_slicer_fb -> Bits -> Daten

            var audioSource = new AudioSource();
            var floatToComplex = new FloatToComplex();
            var muliply = new Multiply();

            connector.Connect(audioSource).To(floatToComplex).To(muliply);


            // Senden:
            // Daten -> Bits -> bytes_to_syms -> interp_fir_filter_fff -> 
            //   frequency_modulator_fc -> multiply_const_cc -> Multiply -> 
            //   Decomplex -> Lautsprecher




            //Application.Run(new Form1());
        }

        static System.Reflection.Assembly CurrentDomain_AssemblyResolve(object sender, ResolveEventArgs args)
        {
            throw new NotImplementedException();
        }
    }
}
