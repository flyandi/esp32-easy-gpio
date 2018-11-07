


/**
 * @ReadFile
 */
String ReadFile(String path) {
  File f = SPIFFS.open(path, "r");   
  if(f) {
    String contents = f.readString();
    f.close();
    return contents;
  }
  return "";
}


/**
 * @LoadConfiguration
 */
bool LoadConfiguration(APConfiguration& apconfig) {

  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.print(" NO FILE ");
    return false;
  }

  size_t size = configFile.size();
  if (size > 2048) {
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(buf.get());

  if (!root.success()) {    
    return false;
  }

  apconfig.ssid = root["ap"]["ssid"];
  apconfig.password = root["ap"]["password"];
  
  return true;
}



String GetWifiStatus()
{
   switch(WiFi.status()) {
    case WL_CONNECTED:      return "Connected";
    case WL_NO_SHIELD:      return "No Shield";
    case WL_IDLE_STATUS:    return "Idle";
    case WL_NO_SSID_AVAIL:  return "No SSID";
    case WL_SCAN_COMPLETED: return "Scan Completed";
    case WL_CONNECT_FAILED: return "Failed";
    case WL_CONNECTION_LOST:return "Lost";
    case WL_DISCONNECTED:   return "Disconnected";
    default:                return "Unknown";
   }
}
