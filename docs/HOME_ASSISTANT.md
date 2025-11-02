# Home Assistant Integration

Guide for integrating the water flow meter with Home Assistant.

## 📋 Prerequisites

- Water flow meter installed and configured
- Zigbee coordinator (Zigbee2MQTT or ZHA dongle)
- Home Assistant installed and running
- Device paired with Zigbee coordinator

## 🔧 Integration Methods

### Method 1: Zigbee2MQTT (Recommended)

#### Setup Zigbee2MQTT

1. **Install Zigbee2MQTT:**
   - Via Home Assistant Add-on Store (recommended)
   - Or standalone installation

2. **Configure Zigbee Dongle:**
   - Connect Zigbee dongle (CC2531, CC2652, etc.)
   - Configure in Zigbee2MQTT settings
   - Start add-on

3. **Pair Device:**
   - Put Zigbee2MQTT in pairing mode ("Permit join")
   - Power on water flow meter
   - Device should appear in Zigbee2MQTT

#### Configure MQTT

1. **Install MQTT Broker:**
   - Home Assistant Add-on: **Mosquitto broker**
   - Enable add-on
   - Create MQTT user/password

2. **Configure Zigbee2MQTT MQTT:**
   - MQTT Server: `core-mosquitto` (if add-on) or `localhost`
   - MQTT Port: `1883`
   - MQTT Base Topic: `zigbee2mqtt` (default)

3. **Install MQTT Integration:**
   - Go to **Settings** → **Devices & Services**
   - Click **Add Integration**
   - Search for **MQTT**
   - Configure MQTT broker

#### Device Discovery

Zigbee2MQTT should auto-discover the device:

- Device appears in **MQTT** integration
- Entities are automatically created:
  - `sensor.water_flow_rate` - Current flow rate (L/min)
  - `sensor.water_total_volume` - Cumulative volume (L)
  - `sensor.water_meter_battery` - Battery percentage (if enabled)
  - `binary_sensor.water_flow_state` - Flow state (flowing/idle)

### Method 2: ZHA (Zigbee Home Automation)

#### Setup ZHA

1. **Install ZHA Integration:**
   - Go to **Settings** → **Devices & Services**
   - Click **Add Integration**
   - Search for **Zigbee Home Automation**
   - Follow setup wizard

2. **Configure Zigbee Dongle:**
   - Select Zigbee coordinator dongle
   - Choose port (e.g., `/dev/ttyUSB0`)
   - Configure radio type (Zigbee 3.0 recommended)
   - Create network

#### Pair Device

1. **Enable Pairing:**
   - Go to **ZHA** integration
   - Click **Add Device**
   - Zigbee pairing mode active

2. **Pair Water Flow Meter:**
   - Power on XIAO ESP32C6
   - Device should join network
   - Confirm pairing in ZHA

3. **Verify Pairing:**
   - Device appears in ZHA devices list
   - Status shows "Online"
   - Entities are created automatically

## 📊 Entity Configuration

### Configure Entities

After pairing, configure entities:

1. **Access Device:**
   - Go to **Settings** → **Devices & Services** → **Zigbee Home Automation**
   - Find **water_flow_meter** device
   - Click to open device page

2. **Configure Entity Names:**
   - Set friendly names for entities
   - Configure icons if desired
   - Set units of measurement

### Entity Details

**Flow Rate Sensor:**
- Entity: `sensor.water_flow_rate`
- Unit: `L/min`
- Icon: `mdi:water-pump`
- Updates: Every 30 seconds when active

**Total Volume Sensor:**
- Entity: `sensor.water_total_volume`
- Unit: `L`
- Icon: `mdi:water-circle`
- Updates: On volume changes or every 30 seconds

**Battery Sensor** (if enabled):
- Entity: `sensor.water_meter_battery`
- Unit: `%`
- Icon: `mdi:battery`
- Updates: Every 10 minutes or on change >5%

**Flow State Binary Sensor:**
- Entity: `binary_sensor.water_flow_state`
- States: `on` (flowing), `off` (idle)
- Icon: `mdi:water`
- Updates: Real-time

## 📱 Dashboard Configuration

### Create Water Flow Card

1. **Add Card to Dashboard:**
   - Edit dashboard
   - Add **Entities** card

2. **Configure Card:**
   ```yaml
   type: entities
   title: Water Flow Meter
   entities:
     - entity: sensor.water_flow_rate
       name: Flow Rate
     - entity: sensor.water_total_volume
       name: Total Volume
     - entity: sensor.water_meter_battery
       name: Battery
     - entity: binary_sensor.water_flow_state
       name: Flow State
   ```

### Create Flow Rate Graph

```yaml
type: history-graph
title: Water Flow Rate
hours_to_show: 24
entities:
  - sensor.water_flow_rate
```

### Create Volume Gauge

```yaml
type: gauge
title: Total Volume
entity: sensor.water_total_volume
min: 0
max: 1000
severity:
  green: 0
  yellow: 500
  red: 900
```

## 🔔 Automations

### Low Flow Alert

```yaml
automation:
  - alias: "Water Flow Low Alert"
    trigger:
      - platform: numeric_state
        entity_id: sensor.water_flow_rate
        below: 0.1
        for: "00:05:00"
    condition:
      - condition: state
        entity_id: binary_sensor.water_flow_state
        state: "on"
    action:
      - service: notify.mobile_app
        data:
          message: "Water flow rate is very low (< 0.1 L/min)"
```

### Volume Milestone Notification

```yaml
automation:
  - alias: "Water Volume Milestone"
    trigger:
      - platform: template
        value_template: "{{ states('sensor.water_total_volume') | float % 100 == 0 }}"
    condition:
      - condition: numeric_state
        entity_id: sensor.water_total_volume
        above: 0
    action:
      - service: notify.mobile_app
        data:
          message: "Water consumption reached {{ states('sensor.water_total_volume') }}L"
```

### Low Battery Warning

```yaml
automation:
  - alias: "Water Meter Low Battery"
    trigger:
      - platform: numeric_state
        entity_id: sensor.water_meter_battery
        below: 25
    action:
      - service: notify.mobile_app
        data:
          message: "Water meter battery is low: {{ states('sensor.water_meter_battery') }}%"
```

## 📊 Energy Monitoring

### Track Water Consumption

Add water meter to energy dashboard:

1. Go to **Settings** → **Dashboards** → **Energy**
2. Add **Water consumption** utility
3. Configure as utility sensor

### Daily Consumption Tracking

```yaml
sensor:
  - platform: statistics
    name: "Daily Water Consumption"
    entity_id: sensor.water_total_volume
    max_age:
      days: 1
    precision: 2
```

## 🔍 Troubleshooting

### Device Not Appearing

**Solutions:**
- Verify Zigbee pairing successful
- Check MQTT connection (if Zigbee2MQTT)
- Verify device is online
- Check Home Assistant logs

### Entities Not Updating

**Solutions:**
- Check Zigbee connection status
- Verify MQTT topics (if Zigbee2MQTT)
- Check device is reporting data
- Review Home Assistant logs

### Values Incorrect

**Solutions:**
- Verify entity configuration
- Check unit conversions
- Test raw MQTT values (if Zigbee2MQTT)
- Verify device calibration

### Device Disconnects Frequently

**Solutions:**
- Check Zigbee signal strength
- Reduce distance to coordinator
- Check for interference
- Verify power supply stable

For more troubleshooting help, see [TROUBLESHOOTING.md](TROUBLESHOOTING.md).

## ✅ Verification Checklist

After integration, verify:

- [ ] Device paired successfully
- [ ] Device shows as "Online" in Home Assistant
- [ ] Entities created and visible
- [ ] Flow rate updates correctly
- [ ] Total volume accumulates
- [ ] Battery level displays (if enabled)
- [ ] Flow state changes correctly
- [ ] Dashboard configured
- [ ] Notifications work (if configured)
- [ ] Automations work (if configured)

## 📚 Additional Resources

- [Installation Guide](INSTALLATION.md)
- [Troubleshooting Guide](TROUBLESHOOTING.md)
- [Zigbee2MQTT Documentation](https://www.zigbee2mqtt.io/)
- [Home Assistant ZHA Integration](https://www.home-assistant.io/integrations/zha/)

---

**Home Assistant integrated?** Your water flow meter is ready to use!

