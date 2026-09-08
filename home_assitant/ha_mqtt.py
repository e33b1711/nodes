

def l_format(id, friendly_name, node_id):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str

def switch_format(id, friendly_name, node_id):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str


def text_format(id, friendly_name, node_id):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "void/{id}"
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str


def siren_format(id, friendly_name, node_id):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    command_template: "{{{{ value }}}}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str


def ro_format(id, friendly_name, node_id):  
    str = f'''
  - unique_id: "{id}"
    name: "{friendly_name}"
    command_topic: "ard_command/{id}"
    position_topic: "ard_state/{id}"
    payload_open: "0"
    payload_close: "100"
    payload_stop: "50"
    position_open: 0
    position_closed: 100
    optimistic: false
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str


def sensor_format(id, friendly_name, node_id, unit = "°C", precision = 0.1, device_class = "temperature"):  
    str = f'''
  - unique_id: "{id}"
    state_class: "measurement"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    value_template: >
      {{% set clean_val = value.strip() | lower %}}
      {{% if clean_val == 'nan' or clean_val == '' %}}
        0
      {{% else %}}
        {{{{ value.strip() }}}}
      {{% endif %}}
    suggested_display_precision: {precision}
    device_class: "{device_class}"
    unit_of_measurement: "{unit}"
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str 


def binary_sensor_format(id, friendly_name, node_id):  
    str = f'''
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    value_template: >
      {{% set clean_val = value.strip() | lower %}}
      {{% if clean_val == 'nan' or clean_val == '' %}}
        0
      {{% else %}}
        {{{{ value.strip() }}}}
      {{% endif %}}
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str


def number_format(item, friendly_name, node_id, unit = "", min = 0, max = 255):  
    str = f'''
  - unique_id: "{item}"
    name: "{friendly_name}"
    command_topic: "ard_command/{item}"
    state_topic: "ard_state/{item}"
    unit_of_measurement: "{unit}"
    min: "{min}"
    max: "{max}"
    optimistic: false
    availability_topic: "nodes/{node_id}/status"
    payload_available: "online"
    payload_not_available: "offline"
    '''
    return str




light_items = [
('LI_EG_WZ',    "Wohnzimmer Mitte",             "eg_ost"),
('LI_EG_WZ_L1', "Wohnzimmer Vorn",              "eg_ost"),
('LI_EG_WZ_L2', "Wohnzimmer Hinten",            "eg_ost"),
('LI_EG_KU_L1', "Küche Arbeitsbeleuchtung",     "eg_west"),
('LI_EG_EZ', 	"Esstisch",                     "eg_west"),
('LI_EG_EZ_L1', "Esszimmer Wandlampen",         "eg_west"),
('LI_EG_EZ_L3', "Esszimmer Leselampe",          "eg_west"),
('LI_EG_EZ_KU', "Esszimmer Grundbeleuchtung",   "eg_west"),
('LI_EG_GA',    "Gang EG",                      "gr"),
('LI_GA_L1',    "Gang EG Wandlampen",           "gr"),
('LI_EG_GR',    "Gardarobe",                    "gr"),
('LI_EG_WC',    "WC",                           "gr"),
('LI_EG_SP',    "Speisekammer",                 "eg_west"),
('LI_OG_BA',    "Badezimmer",                   "og_ost"),
('LI_OG_KN',    "Leo",                          "og_west"),
('LI_OG_KN_L1', "Leo Spots",                    "og_west"),
('LI_OG_KS', 	"Bini",                         "og_west"),
('LI_OG_KS_L1', "Bini Spots",                   "og_west"),
('LI_OG_SZ', 	"Schlafzimmer",                 "og_ost"),
('LI_OG_SZ_L1', "Leselampe Melli",              "og_ost"),
('LI_OG_SZ_L2', "Leselampe Anselm",             "og_ost"),
('LI_OG_GA', 	"Gang OG",                      "og_west"),
('LI_OG_GA_L1', "Gang OG Spots",                "og_west"),
('LI_UG_HO', 	"Hobby Schreibtisch",           "ug"),
('LI_UG_WK',    "Waschküche",                   "ug"),
('LI_UG_HK',    "Technik",                      "ug"),
('LI_UG_GA',    "Gang UG",                      "ug"),
('LI_UG_HN',    "Hobby Hinten",                 "ug"),
('LI_UG_TR',    "UG Treppe",                    "eg_ost"),
('LI_EG_VH',    "Vorhaus",                      "eg_ost"),
('ZE_EG_VH',    "Vorhaus Timer",                "eg_ost"),
('LI_EG_AS',    "Außen Süd",                    "eg_ost"),
('LI_EG_AW',    "Außen West",                   "eg_ost"),
('LI_EG_AO',    "Außen Ost",                    "eg_ost"),
('LI_GR',       "Garage",                       "gr"),
('LI_GR_L1',    "Garage Außen",                 "gr"),
('ZE_GR_1',	    "Garage Timer 10 min",          "gr"),
('ZE_GR_2',	    "Garage Timer außen",           "gr"),

('LI_TEST_04',  "LI_TEST_04",        "test"),
('LI_TEST_05',  "LI_TEST_05",        "test"),
('LI_TEST_06',	"LI_TEST_06",        "test"),
('LI_TEST_07',	"LI_TEST_07",        "test"),
]

siren_items = [
('BELL',        "Klingel (dauer)",   "ug"),
('ZE_BELL',     "Klingel",          "ug"),
]

ro_items = [
("RO_EG_SU", "Rollo Esszimmer Süd",                 "eg_west"),
("RO_EG_WE", "Rollo Esszimmer West",                "eg_west"),
("RO_OG_BA", "Rollo Bad",                           "og_ost"),
("DF_OG_GA", "Dachfenster Gang",                    "og_west"),
("VD_OG_GA", "Verdunkelung Gang",                   "og_west"),
("RO_OG_KN", "Rollo Leo",                           "og_west"),
("VD_OG_KN", "Verdunkelung Leo",                    "og_west"),
("DF_OG_KN", "Dachfenster Leo",                     "og_west"),
("RO_OG_KS", "Rollo Bini",                          "og_west"),
("VD_OG_KS", "Verdunkelung Bini",                   "og_west"),
("DF_OG_KS", "Dachfenster Bini",                    "og_west"),
("RO_OG_SZ", "Rollo Schlafzimmer",                  "og_ost"),
("VD_OG_SZ", "Verdunkelung Schlafzimmer",           "og_ost"),
("DF_OG_SZ", "Dachfenster Schlafzimmer",            "og_ost"),
("DO_GR",    "Garagentor",                          "gr"),
]  

binary_sensor_items =  [
("F_HE",     "Gastherme", ""),
("F_WW",     "Warnung Pumpensumpf", ""),
("F_RAIN",   "Regen", ""),
]

sensor_items = [
("TI_PU_O",  "Temperatur Puffer oben",     "ug"),   
("TI_PU_U",  "Temperatur Puffer unten",    "ug"),  
("TI_EG_WZ", "Temperatur Wohnzimmer",      "eg_west"),      
("TI_EG_KU", "Temperatur Küche",           "eg_west"),           
("TI_EG_EZ", "Temperatur Esszimmer",       "eg_west"),      
("TI_EG_GA", "Temperatur Gang EG",         "eg_west"),             
("TI_OG_BA", "Temperatur Badezimmer",      "og_ost"),             
("TI_OG_KN", "Temperatur Leo",             "og_west"),         
("TI_OG_KS", "Temperatur Bini",            "og_west"),        
("TI_OG_SZ", "Temperatur Schlafzimmer",    "og_ost"),                  
("TI_OG_GA", "Temperatur Gang OG",         "og_west"),        
("TI_UG_HO", "Temperatur Hobby",           "ug"),       
("TI_UG_WK", "Temperatur Waschküche",      "ug"),            
("TI_UG_HK", "Temperatur Technik",         "ug"),           
("TI_UG_GA", "Temperatur Gang UG",         "ug"),           
("TI_UG_LA", "Temperatur Lager",           "ug"),      
("TI_GR",    "Temperatur Garage",          "gr"),         
("TI_AU",    "Temperatur Außen",           "gr"),         
("HI_EG_WZ", "Feuchte Wohnzimmer",         "eg_west"),         
("HI_EG_KU", "Feuchte Küche",              "eg_west"),     
("HI_EG_EZ", "Feuchte Esszimmer",          "eg_west"),          
("HI_EG_GA", "Feuchte Gang EG",            "eg_west"),       
("HI_OG_BA", "Feuchte Badezimmer",         "og_ost"),      
("HI_OG_KN", "Feuchte Leo",                "og_west"),    
("HI_OG_KS", "Feuchte Bini",               "og_west"),    
("HI_OG_SZ", "Feuchte Schlafzimmer",       "og_ost"),           
("HI_OG_GA", "Feuchte Gang OG",            "og_west"),     
("HI_UG_HO", "Feuchte Hobby",              "ug"),   
("HI_UG_WK", "Feuchte Waschküche",         "ug"),      
("HI_UG_HK", "Feuchte Technik",            "ug"),      
("HI_UG_GA", "Feuchte Gang UG",            "ug"),     
("HI_UG_LA", "Feuchte Lager",              "ug"),   
("HI_GR",    "Feuchte Garage",             "gr"),     
("HI_AU",    "Feuchte Außen",              "gr"),    
]

number_items = [
("U_EL",     "Elektrische Heizung", "ug"),
]

switch_items = [
("V_OG_KS",  "Heizventil Bini",             "og_ost"),
("V_OG_KN",  "Heizventil Leo",              "og_ost"),
("V_OG_GA",  "Heizventil Gang OG",          "og_ost"),
("V_OG_BA",  "Heizventil Badezimmer",       "og_ost"),
("V_OG_SZ",  "Heizventil Schlafzimmer",     "og_ost"),
("V_UG_HO",  "Heizventil Hobby",            "ug"),
("V_UG_H2",  "Heizventil Hobby",            "ug"),
("V_UG_LA",  "Heizventil Lager",            "ug"),
("V_UG_GA",  "Heizventil Gang UG",          "ug"),
("V_UG_WK",  "Heizventil Waschküche",       "ug"),
("V_EG_KU",  "Heizventil Küche",            "eg_ost"),
("V_EG_EZ",  "Heizventil Esszimmer",        "eg_ost"),
("V_EG_E2",  "Heizventil Esszimmer",        "eg_ost"),
("V_EG_GA",  "Heizventil Gang EG",          "eg_ost"),
("V_EG_WZ",  "Heizventil Wohnzimmer",       "eg_ost"),
("V_EG_GR",  "Heizventil Gardarobe",        "eg_ost"),
("V_EG_WC",  "Heizventil WC",               "eg_ost"),
]


text_items = [
("og_west",	       "revision_og_west",         "og_west"	     ),
("og_ost", 	       "revision_og_ost",          "og_ost"	         ),
("eg_west",	       "revision_eg_west",         "eg_west"	     ),
("eg_ost", 	       "revision_eg_ost",          "eg_ost"	         ),
("ug",     	       "revision_ug",              "ug"    	         ),
("gr", 	           "revision_garage",          "gr"	             ),
("relay_service",  "revision_relay_service",   "relay_service"   ),
("bridge_service", "revision_bridge_service",  "bridge_service"  ),
]




if __name__ == "__main__":


    with open("mqtt.yaml", "w", encoding="utf-8") as fh:

        fh.write("\n\n- light:\n")
        for item in light_items:
            fh.write(l_format(item[0],item[1], item[2]))

        fh.write("\n\n- siren:\n")
        for item in siren_items:
            fh.write(siren_format(item[0],item[1], item[2]))
        
        fh.write("\n\n- cover:\n")
        for item in ro_items:
            fh.write(ro_format(item[0], item[1], item[2]))

        fh.write("\n\n- sensor:\n")
        for item in sensor_items:
            if item[0].startswith("TI_"):
                fh.write(sensor_format(item[0], item[1], item[2], "°C", 0.1, "temperature"))
            elif item[0].startswith("HI_"):
                fh.write(sensor_format(item[0], item[1], item[2], "%", 1, "humidity"))

        fh.write("\n\n- binary_sensor:\n")
        for item in binary_sensor_items:
            fh.write(binary_sensor_format(item[0], item[1], item[2]))

        fh.write("\n\n- text:\n")
        for item in text_items:
            fh.write(text_format(item[0], item[1], item[2]))

        fh.write("\n\n- number:\n")
        for item in number_items:
            fh.write(number_format(item[0], item[1], item[2]))

        fh.write("\n\n- switch:\n")
        for item in switch_items:
            fh.write(switch_format(item[0], item[1], item[2]))