#include "supabaseESP32.h"
#include "supabase_secrets.h"
// #if ESP32
HTTPClient http;
// #endif



CSupabase::CSupabase()
{
    _supabase_url = supabaseUrl;
    _supabase_key = supabaseApiKey;    
}


std::string CSupabase::UPDATE(std::string tableName,std::string supabase_data)
{
    std::string url = _supabase_url + tableName;
    http.begin(url.c_str());
    http.addHeader("apikey", _supabase_key.c_str());
    http.addHeader("Authorization", _supabase_bearer.c_str());
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "resolution=merge-duplicates");
    int httpResponseCode = http.POST(supabase_data.c_str());
    String payload = http.getString();
    Serial.println(payload);
    http.end();
    std::string str;
    str=payload.c_str();
    return str;
}

std::string CSupabase::INSERT(std::string tableName,std::string supabase_data)
{    
    Serial.println("Insert");
    Serial.println(supabase_data.c_str());
    std::string url = _supabase_url + tableName;
    Serial.println(url.c_str());
    http.begin(url.c_str());    
    http.addHeader("apikey", _supabase_key.c_str());
    http.addHeader("Authorization", _supabase_bearer.c_str());
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "return=representation");
    int httpResponseCode = http.POST(supabase_data.c_str());
    String payload = http.getString();
    Serial.print("payload");    Serial.println(payload);
    Serial.println(httpResponseCode);
    http.end();
    std::string str;
    str=payload.c_str();
    return str;
} 


std::string CSupabase::INSERT(std::string tableName,CMsg msg){

    std::string str=msg.get(_BASE);
    std::string Mid=msg.get(_MID);
    std::string Bsid=msg.get("Bsid");
    std::string Name=msg.get(_NAME);
    std::string T=msg.get(_TIME);
    std::string F=msg.get(_FROM);


    for (auto x:msg.Parameters){

        if((x.first=="N")||(x.first=="MID")||(x.first=="Bsid")||(x.first=="PWD")||(x.first=="SD")||(x.first=="RTS")||(x.first=="T")||(x.first=="F")) {
            continue;
        }

        CMsg m;
        m.set("Mid",Mid);
        m.set("Bsid",Bsid);
        m.set("Name",Name);
        m.set("T",T);
        m.set("From",F);
        m.set("Key", x.first);
        m.set("Value" ,x.second);
        std::string data = m.payload();
        INSERT(tableName,data);
    }

    CMsg m;
    m.set("Mid",Mid);
    m.set("Bsid",Bsid);
    m.set("Name",Name);
    m.set("T",T);
    m.set("From",F);
    m.set("Key", std::string("RAW_")+F+std::string("_")+Name);
    m.set("Value" ,msg.serialize());
    std::string data = m.payload();
    INSERT(tableName,data);


    str="";
    return str;
}


std::string CSupabase::SELECT(std::string tableName,CMsg mequal,CMsg mless, CMsg mmore, int limit)
{
    std::string supabase_data;
    
    String column1="'Id'";
    String column2="Id";

    String value1="'Id'";
    String value2="5b665d6e-9cb7-43f3-8040-4eda203d3c36";

    String url = _supabase_url.c_str();
    url+=  tableName.c_str();
    url+=String("?limit=")+String(limit)+String("&");
 //   url+="?select=*"&gte(" + String(column_name) + "," + String(min_value) + ")&lte(" + String(column_name) + "," + String(max_value) + ")";
 
    //?select=*&and(eq(" + String(column_name1) + "," + String(value1) + "),eq(" + String(column_name2) + "," + String(value2) + "))";    
    //url+="?select=*&eq.(" + column1 + "," + value1 + ")";   
 
   //url+="Id=eq.5b665d6e-9cb7-43f3-8040-4eda203d3c36&Key=eq.""Mary""";// + String(id);
   
   std::string sequal=mequal.supabase();
   std::string sless=mless.supabase();
   std::string smore=mmore.supabase();
   
   if(sequal.length()>0)
   {
       url+=String(sequal.c_str())+String("&");
   }

   if(sless.length()>0)
   {
       url+=String(sless.c_str())+String("&");
   }
   
   if(smore.length()>0)
   {
       url+=String(smore.c_str())+String("&");
   }
    
    Serial.println(url);
    http.begin(url.c_str());    
    http.addHeader("apikey", _supabase_key.c_str());
    http.addHeader("Authorization", _supabase_bearer.c_str());
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "return=representation");
    
    int httpResponseCode = http.GET();
    String payload = http.getString();
    http.end();
    Serial.println(payload);

    std::string str=payload.c_str();
    return str;
    //return httpResponseCode;
}


std::string CSupabase::DELETE(std::string tableName,std::string id) {   // Validado
    std::string url = _supabase_url + tableName;
    url +=  std::string("?Id=eq.") + id;
	http.begin(url.c_str());
    
    http.addHeader("apikey", _supabase_key.c_str());
    http.addHeader("Authorization", _supabase_bearer.c_str());
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Prefer", "return=representation");
	int httpResonseCode = http.sendRequest("DELETE");
	//processResponse(httpCode, http);
    http.end();
    std::string str;
    return str;
}


/*
void CSupabase::processResponse(int httpCode, HTTPClient& http)
{
	if (httpCode > 0) {
		Serial.printf("Código de respuesta: %d\t", httpCode);
   
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            // Serial.println(payload);
            char documentoJson[500];
            payload.replace(" ", "");
            payload.replace("\n", "");
            payload.trim();
            payload.remove(0,1);
            payload.remove(payload.length()-1, 1);
            payload.toCharArray(documentoJson, 500);

            Serial.println(payload);
            
            StaticJsonDocument<200> doc;
            deserializeJson( doc, documentoJson);
		}
	}
	else {
		Serial.printf("Ha ocurrido un error en la solicitud: %s\n", http.errorToString(httpCode).c_str());
	}
	http.end();
}
*/