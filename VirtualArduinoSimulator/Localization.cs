using System;
using System.Resources;

namespace VirtualArduinoSimulator
{
    /// <summary>
    /// Třída pro lokalizaci konzole
    /// </summary>
    public static class Localization
    {
        /// <summary>
        /// ResourceManager pro získání řetězců z Resource Files.
        /// </summary>
        private static readonly ResourceManager ResourceManager = new ResourceManager(typeof(Resources.Strings));
        
        /// <summary>
        /// Vrací lokalizovaný řetězec 
        /// </summary>
        /// <param name="key">Klíč</param>
        /// <param name="args">Proměné pro vložení do textu</param>       
        public static string getString(string key, params object[] args)
        {
            string format = ResourceManager.GetString(key);
            return string.Format(format, args);
        }
        /// <summary>
        /// lokalizovaný WriteLine jako extension metoda stringu
        /// </summary>
        public static void printLocalizedLine(this string key, params object[] args)
        {
            Console.WriteLine(getString(key, args));
        }
        /// <summary>
        /// lokalizovaný print
        /// </summary>
        public static void printLocalized(this string key, params object[] args)
        {
            Console.Write(getString(key, args));
        }
    }
}
