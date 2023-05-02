using System;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;
using Newtonsoft.Json.Linq;
//using loc = VirtualArduinoSimulator.Localization; // alias

namespace VirtualArduinoSimulator
{
    /// <summary>
    /// Program pro simulaci Bluetooth komunikace s meteostanicí
    /// </summary>
    internal class Program
    {
        
        static void Main(string[] args)
        {
            "version".printLocalizedLine(System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString());           
            "info".printLocalizedLine();
            Console.WriteLine();

            IsBluetoothEnabled();

            var serviceClass = BluetoothService.SerialPort;
            var listener = new BluetoothListener(serviceClass);
            listener.Start();

            var radio = BluetoothRadio.Default.LocalAddress;
            
            "listening".printLocalizedLine(radio.ToString("C"), serviceClass);

            while (true)
            {
                var client = listener.AcceptBluetoothClient();
                _ = Task.Run(() => HandleClient(client));
            }

        }
        /// <summary>
        /// Obsluha spojení a odesílání zpráv
        /// </summary>
        private static void HandleClient(BluetoothClient client)
        {
            "connected".printLocalizedLine();
            
            using (client)
            {
                var stream = client.GetStream();
                var reader = new StreamReader(stream, Encoding.ASCII);
                var writer = new StreamWriter(stream, Encoding.ASCII) { AutoFlush = true };

                while (client.Connected)
                {
                    int receivedValue = reader.Read();

                    if (receivedValue == -1)
                    {
                        break; // pro disconnected

                    }
                    switch (receivedValue) // musí odpovídat kódu na desce!!
                    {
                        case '0':
                            "LED Off".printLocalizedLine();
                            break;
                        case '1':
                            "LED ON".printLocalizedLine();
                             break;
                        case '2':
                            "sended via Bluetooth".printLocalizedLine();
                            var jsonString = generateRandomJSONdataObject();
                            writer.Write(jsonString);
                            Console.WriteLine(jsonString);
                            break;
                        case 13:
                            break;
                        case 10:
                            break; // ignoruj konce řádku
                        default:
                            "Unknow command".printLocalizedLine(receivedValue);
                            break;
                    }
                }
            }
            "disconnected".printLocalizedLine();
        }

        /// <summary>
        /// vygeneruje vzorový JSON objekt (musí odpovídat kódu na desce)
        /// </summary>
        private static string generateRandomJSONdataObject()
        {
            var random = new Random();
            double airTemperature = random.Next(-15, 11) + random.NextDouble();
            double snowTemperature = random.Next(-15, 11) + random.NextDouble();
            double humidity = random.Next(20, 91) + random.NextDouble();

            var jsonDoc = new JObject
            {
                ["airTemperature"] = Math.Round(airTemperature, 1),
                ["snowTemperature"] = Math.Round(snowTemperature, 1),
                ["humidity"] = Math.Round(humidity, 1)
            };

            return jsonDoc.ToString();
           
        }
        /// <summary>
        /// kontroluje, zda je Bluetooth zapnuto
        /// </summary>
        public static bool IsBluetoothEnabled()
        {
            var radio = BluetoothRadio.Default;
            if (radio == null)
            {
                "No Bluetooth adapter found".printLocalizedLine();
                return false;
            }

            if (radio.Mode == RadioMode.PowerOff)
            {
                "Bluetooth is turned off".printLocalizedLine();
                return false;
            }

            "Bluetooth is enabled".printLocalizedLine();
            return true;
        }


    }

  

}
