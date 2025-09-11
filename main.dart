import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:permission_handler/permission_handler.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await SystemChrome.setPreferredOrientations([
    DeviceOrientation.portraitUp,
  ]);
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      debugShowCheckedModeBanner: false,
      home: BluetoothPage(),
    );
  }
}

class BluetoothPage extends StatefulWidget {
  const BluetoothPage({super.key});

  @override
  BluetoothPageState createState() => BluetoothPageState();
}

class BluetoothPageState extends State<BluetoothPage> {
  BluetoothConnection? connection;
  bool isConnected = false;
  List<String> messages = [];
  List<BluetoothDevice> devices = [];
  BluetoothDevice? selectedDevice;
  bool loadingDevices = true;

  @override
  void initState() {
    super.initState();
    initBluetooth();
  }

  Future<void> initBluetooth() async {
    await requestPermissions();
    await getBondedDevices();
    setState(() {
      loadingDevices = false;
    });
  }

  Future<void> requestPermissions() async {
    await Permission.bluetooth.request();
    await Permission.bluetoothScan.request();
    await Permission.bluetoothConnect.request();
    await Permission.location.request();
  }

  Future<void> getBondedDevices() async {
    List<BluetoothDevice> devicesTemp = [];
    try {
      devicesTemp = await FlutterBluetoothSerial.instance.getBondedDevices();
    } catch (e) {
      debugPrint("Errore nel recupero dispositivi: $e");
    }
    setState(() {
      devices = devicesTemp;
    });
  }

  void connectToESP32() async {
    if (isConnected || selectedDevice == null) return;
    try {
      final BluetoothConnection connectionTemp =
          await BluetoothConnection.toAddress(selectedDevice!.address);
      connection = connectionTemp;
      setState(() => isConnected = true);

      connection!.input!.listen((data) {
        final message = String.fromCharCodes(data);
        setState(() {
          messages.add(message.trim());
          if (messages.length > 50) messages.removeAt(0);
        });
      }).onDone(() {
        setState(() {
          isConnected = false;
          connection = null;
        });
        if (!mounted) return;
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Connessione terminata dall\'ESP32')),
        );
      });
    } catch (e) {
      debugPrint('Errore connessione: $e');
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('Errore di connessione: $e')),
      );
    }
  }

  void disconnectFromESP32() {
    if (connection != null && isConnected) {
      connection!.finish();
      setState(() {
        isConnected = false;
        connection = null;
      });
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Disconnesso dall\'ESP32')),
      );
    }
  }

  void sendCommand(String command) {
    if (connection != null && isConnected) {
      connection!.output.add(Uint8List.fromList("$command\n".codeUnits));
      connection!.output.allSent;
      debugPrint('Inviato: $command');
    } else {
      debugPrint('Non connesso!');
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Non connesso!')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Allarme Camper")),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              const SizedBox(height: 10),
              Text(
                isConnected ? "Stato: Connesso ✅" : "Stato: Non connesso ❌",
                style: const TextStyle(fontSize: 22, fontWeight: FontWeight.bold),
                textAlign: TextAlign.center,
              ),
              const SizedBox(height: 20),
              loadingDevices
                  ? const Center(child: CircularProgressIndicator())
                  : devices.isEmpty
                      ? const Padding(
                          padding: EdgeInsets.all(16),
                          child: Text(
                            "Nessun dispositivo Bluetooth accoppiato.\nVai nelle impostazioni Bluetooth e collega l'ESP32.",
                            style: TextStyle(fontSize: 16),
                            textAlign: TextAlign.center,
                          ),
                        )
                      : Column(
                          crossAxisAlignment: CrossAxisAlignment.stretch,
                          children: [
                            const Text(
                              "Seleziona dispositivo:",
                              style: TextStyle(fontSize: 16),
                            ),
                            DropdownButton<BluetoothDevice>(
                              hint: const Text("Scegli dispositivo"),
                              value: selectedDevice,
                              isExpanded: true,
                              items: devices.map((device) {
                                return DropdownMenuItem(
                                  value: device,
                                  child: Text("${device.name} (${device.address})"),
                                );
                              }).toList(),
                              onChanged: isConnected
                                  ? null
                                  : (device) {
                                      setState(() {
                                        selectedDevice = device;
                                      });
                                    },
                            ),
                          ],
                        ),
              Row(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  ElevatedButton(
                    onPressed: isConnected || selectedDevice == null ? null : connectToESP32,
                    child: const Text("Connetti"),
                  ),
                  const SizedBox(width: 20),
                  ElevatedButton(
                    onPressed: isConnected ? disconnectFromESP32 : null,
                    style: ElevatedButton.styleFrom(backgroundColor: Colors.red),
                    child: const Text("Disconnetti"),
                  ),
                ],
              ),
              const SizedBox(height: 18),
              ElevatedButton(
                onPressed: isConnected ? () => sendCommand('1') : null,
                child: const Text("Inserisci Allarme (LED rosso)"),
              ),
              ElevatedButton(
                onPressed: isConnected ? () => sendCommand('0') : null,
                child: const Text("Disinserisci Allarme (LED verde)"),
              ),
              ElevatedButton(
                onPressed: isConnected ? () => sendCommand('T') : null,
                style: ElevatedButton.styleFrom(backgroundColor: Colors.amber),
                child: const Text("Test Clacson"),
              ),
              const SizedBox(height: 18),
              const Text(
                "Messaggi da ESP32:",
                style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
              ),
              Container(
                height: 180,
                margin: const EdgeInsets.only(top: 8),
                padding: const EdgeInsets.all(8),
                decoration: BoxDecoration(
                  color: Colors.black12,
                  borderRadius: BorderRadius.circular(8),
                ),
                child: ListView.builder(
                  itemCount: messages.length,
                  itemBuilder: (context, index) {
                    return Text(
                      messages[index],
                      style: const TextStyle(fontSize: 16),
                    );
                  },
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}