// #ifndef LED13_H
// #define LED13_H
#include <WiFi.h>
#include <HTTPClient.h>

#include <msg.h>

class CSupabase{
    std::string _supabase_url;
    std::string _supabase_key;
    std::string _supabase_bearer= "Bearer ";
public:
    CSupabase();
    ~CSupabase(){};
    
    
    std::string UPDATE(std::string tableName,std::string supabase_data);  
    std::string INSERT(std::string tableName,std::string supabase_data);  
    std::string INSERT(std::string tableName,CMsg msg);  

    std::string DELETE(std::string tableName,std::string id);
    std::string SELECT(std::string tableName,CMsg mequal,CMsg mless, CMsg mmore, int limit=5);
    
};