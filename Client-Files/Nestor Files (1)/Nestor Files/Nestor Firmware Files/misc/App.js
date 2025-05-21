import React, { useEffect, useState } from 'react';
import { Text, View, StyleSheet } from 'react-native';
import { BleManager } from 'react-native-ble-plx';

const READINESS_UUID = '2a3f';

export default function App() {
  const [readiness, setReadiness] = useState(null);
  const manager = new BleManager();

  useEffect(() => {
    const subscription = manager.onStateChange((state) => {
      if (state === 'PoweredOn') scanAndConnect();
    }, true);

    return () => subscription.remove();
  }, []);

  const scanAndConnect = () => {
    manager.startDeviceScan(null, null, (error, device) => {
      if (error) return;
      if (device.name?.includes("Nestor")) {
        manager.stopDeviceScan();
        device.connect()
          .then((d) => d.discoverAllServicesAndCharacteristics())
          .then((d) => {
            d.monitorCharacteristicForService(
              '180d',
              READINESS_UUID,
              (err, char) => {
                if (char?.value) {
                  const value = Buffer.from(char.value, 'base64').readUInt8(0);
                  setReadiness(value);
                }
              }
            );
          });
      }
    });
  };

  const readinessColor = readiness >= 80 ? 'green' :
                         readiness >= 50 ? 'gold' :
                         readiness >= 30 ? 'orange' : 'red';

  return (
    <View style={styles.container}>
      <Text style={[styles.text, { color: readinessColor }]}>
        Readiness Score: {readiness ?? '...'}
      </Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, justifyContent: 'center', alignItems: 'center' },
  text: { fontSize: 30, fontWeight: 'bold' },
});
