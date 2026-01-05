

def l_format(id, friendly_name):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    '''
    return str

def fan_format(id, friendly_name):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    '''
    return str

def text_format(id, friendly_name):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "void/{id}"
    '''
    return str


def siren_format(id, friendly_name):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    '''
    str2 = '''command_template: "{{ value }}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    '''
    return str + str2

def valve_format(id, friendly_name):  
    str = f'''  
  - unique_id: "{id}"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    command_topic: "ard_command/{id}"
    payload_open: "1"
    payload_close: "0"
    state_open: "1"
    state_closed: "0"
    optimistic: false
    '''
    return str

    

def ro_format(id, friendly_name):  
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
    '''
    return str



def sensor_format(id, friendly_name, unit = "°C", precision = 0.1, device_class = "temperature"):  
    str = f'''
  - unique_id: "{id}"
    state_class: "measurement"
    name: "{friendly_name}"
    state_topic: "ard_state/{id}"
    suggested_display_precision: {precision}
    device_class: "{device_class}"
    unit_of_measurement: "{unit}"
    '''
    return str

def number_format(item, friendly_name, unit = "", min = 0, max = 255):  
    str = f'''
  - unique_id: "{item}"
    name: "{friendly_name}"
    command_topic: "ard_command/{item}"
    state_topic: "ard_state/{item}"
    unit_of_measurement: "{unit}"
    min: "{min}"
    max: "{max}"
    optimistic: false
    '''
    return str


def thermos_format(id, friendly_name, min = 10, max = 30):
    str = f'''
  - unique_id: "TS_{id}"
    name: "{friendly_name}"
    modes:
      - "off"
      - "heat"
    temperature_command_topic: "ard_command/TS_{id}"
    temperature_state_topic: "ard_state/TS_{id}"
    mode_command_topic: "ard_command/TM_{id}"
    mode_state_topic: "ard_state/TM_{id}"
    min_temp: "{min}"
    max_temp: "{max}"
    optimistic: false'''
    str2 = '''
    mode_state_template: >-
      {% set modes = { '0':'off', '1':'heat' } %}
      {{ modes[value] if value in modes else 'off' }}
    mode_command_template: >-
        {% set modes = { 'off':'0', 'heat':'1' } %}
        {{ modes[value] if value in modes else '0' }}

    '''
    return str + str2

# TODO revision strings / availibilty warning
# TODO something like post all rule



light_items = [
('LI_EG_WZ',    "Wohnzimmer Mitte"),	 
('LI_EG_WZ_L1', "Wohnzimmer Vorn"),
('LI_EG_WZ_L2', "Wohnzimmer Hinten"),
('LI_EG_KU_L1', "Küche Arbeitsbeleuchtung"),
('LI_EG_EZ', 	  "Esstisch"),
('LI_EG_EZ_L1', "Esszimmer Wandlampen"),
('LI_EG_EZ_L3', "Esszimmer Leselampe"),
('LI_EG_EZ_KU', "Esszimmer Grundbeleuchtung"),
('LI_EG_GA',    "Gang EG"),
('LI_GA_L1',    "Gang EG Wandlampen"),
('LI_EG_GR',    "Gardarobe"),
('LI_EG_WC',    "WC"),
('LI_EG_SP',    "Speisekammer"),
('LI_OG_BA',    "Badezimmer"),
('LI_OG_KN',    "Leo"),
('LI_OG_KN_L1', "Leo Spots"),
('LI_OG_KS', 	  "Bini"),
('LI_OG_KS_L1', "Bini Spots"),
('LI_OG_SZ', 	  "Schlafzimmer"),
('LI_OG_SZ_L1', "Leselampe Melli"),
('LI_OG_SZ_L2', "Leselampe Anselm"),
('LI_OG_GA', 	  "Gang OG"),
('LI_OG_GA_L1', "Gang OG Spots"),
('LI_UG_HO', 	  "Hobby Schreibtisch"),
('LI_UG_WK',    "Waschküche"),
('LI_UG_HK',    "Technik"),
('LI_UG_GA',    "Gang UG"),
('LI_UG_HN',    "Hobby Hinten"),
('LI_UG_TR',    "UG Treppe"),
('LI_EG_VH',    "Vorhaus"),
('ZE_EG_VH',    "Vorhaus Timer"),
('LI_EG_AS',    "Außen Süd"),
('LI_EG_AW',    "Außen West"),
('LI_EG_AO',    "Außen Ost"),
('LI_GR',       "Garage"), 
('LI_GR_L1',    "Garage Außen"),
('ZE_GR_0',	    "Garage Timer 10min"),
('ZE_GR_1',	    "Garage Timer 2 min"),
('ZE_GR_2',	    "Garage Timer außen"),
('LI_CH_L2',    "Licht Hühnerhaus Zaun"),
('LI_CH_L3',    "Hühnerhaus (L3)"),
('LI_CH_L4',    "Hühnerhaus (L4)"),
('LI_SA_L1',    "Sauna (L1)"),
('LI_SA_L2',    "Sauna (L2)"),
('LI_SA_L3',    "Sauna (L3)"),
]

siren_items = [
('BELL',        "Klingel (dauer)"), 
('ZE_BELL',     "Klingel"), 
]

valve_items=[
('PUMP', "Zisterne"), 
]

fan_items=[
('LI_CH',       "Lüftung Hühnerhaus"),  
]

ro_items = [
("RO_EG_SU", "Rollo Esszimmer Süd"),
("RO_EG_WE", "Rollo Esszimmer West"),
("RO_OG_BA", "Rollo Bad"),  
("DF_OG_GA", "Dachfenster Gang"),  
("VD_OG_GA", "Verdunkelung Gang"),  
("RO_OG_KN", "Rollo Leo"),  
("VD_OG_KN", "Verdunkelung Leo"),  
("DF_OG_KN", "Dachfenster Leo"),
("RO_OG_KS", "Rollo Bini"),
("VD_OG_KS", "Verdunkelung Bini"),
("DF_OG_KS", "Dachfenster Bini"),
("RO_OG_SZ", "Rollo Schlafzimmer"),
("VD_OG_SZ", "Verdunkelung Schlafzimmer"),
("DF_OG_SZ", "Dachfenster Schlafzimmer"),
("DO_GR",    "Garagentor"),
("DO_CH",    "Hühnerklappe"),
]  


sensor_items = [
("TI_PU_O ", "Puffer oben", ""),
("TI_PU_U ", "Puffer unten", ""),
("F_HE",     "Gastherme", ""),
("F_WW",     "Warnung Pumpensumpf", ""),
("F_RAIN",   "Regen", ""),
("TI_EG_WZ", "Wohnzimmer", ""),
("TI_EG_KU", "Küche", ""),
("TI_EG_EZ", "Esszimmer", ""),
("TI_EG_GA", "Gang EG", ""),
("TI_OG_BA", "Badezimmer", ""),
("TI_OG_KN", "Leo", ""),
("TI_OG_KS", "Bini", ""),
("TI_OG_SZ", "Schlafzimmer", ""),
("TI_OG_GA", "Gang OG", ""),
("TI_UG_HO", "Hobby", ""),
("TI_UG_WK", "Waschküche", ""),
("TI_UG_HK", "Technik", ""),
("TI_UG_GA", "Gang UG", ""),
("TI_UG_LA", "Lager", ""),
("TI_GR",    "Garage", ""),
("TI_AU",    "Außen", ""),
("TI_CH", 	 "Hühnerhaus", ""),  
("TI_SA",    "Sauna", ""),
("TI_SA1",   "Sauna oben/hinten", ""),
("TI_SA2",   "Sauna mitte", ""),
("TI_SA3",   "Sauna unten", ""),
("TI_SA4",   "Sauna außen", ""), 
("HI_EG_WZ", "Wohnzimmer", ""),
("HI_EG_KU", "Küche", ""),
("HI_EG_EZ", "Esszimmer", ""),
("HI_EG_GA", "Gang EG", ""),
("HI_OG_BA", "Badezimmer", ""),
("HI_OG_KN", "Leo", ""),
("HI_OG_KS", "Bini", ""),
("HI_OG_SZ", "Schlafzimmer", ""),
("HI_OG_GA", "Gang OG", ""),
("HI_UG_HO", "Hobby", ""),
("HI_UG_WK", "Waschküche", ""),
("HI_UG_HK", "Technik", ""),
("HI_UG_GA", "Gang UG", ""),
("HI_UG_LA", "Lager", ""),
("HI_GR",    "Garage", ""),
("HI_AU",    "Außen", ""),
("HI_CH",    "Hühnerhaus", ""),
]

number_items = [
("V_UG_HO",  "Hobby", ""),
("V_UG_LA",  "Lager", ""),
("V_UG_GA",  "Gang UG", ""),
("V_UG_WK",  "Waschküche", ""),
("V_EG_KU",  "Küche", ""),
("V_EG_EZ",  "Esszimmer", ""),
("V_EG_GA",  "Gang EG", ""),
("V_EG_WZ",  "Wohnzimmer", ""),
("V_EG_GR",  "Gardarobe", ""),
("V_EG_WC",  "WC", ""),
("V_OG_KS",  "Bini", ""),
("V_OG_KN",  "Leo", ""),
("V_OG_GA",  "Gang OG", ""),
("V_OG_BA",  "Badezimmer", ""),
("V_OG_SZ",  "Schlafzimmer", ""),
("V_SA",     "Sauna", ""),
("U_EL",     "Elektrische Heizung", ""),
]



thermos_items = [
("UG_HO", "Klima Hobby", 10, 30),     
("UG_LA", "Klima Lager", 10, 30),     
("UG_GA", "Klima Gang UG", 10, 30),     
("UG_WK", "Klima Waschküche", 10, 30),     
("EG_KU", "Klima Küche", 10, 30),     
("EG_EZ", "Klima Esszimmer", 10, 30),     
("EG_GA", "Klima Gang EG", 10, 30),     
("EG_WZ", "Klima Wohnzimmer", 10, 30),        
("OG_KS", "Klima Bini", 10, 30),     
("OG_KN", "Klima Leo", 10, 30),     
("OG_GA", "Klima Gang OG", 10, 30),     
("OG_BA", "Klima Bad", 10, 30),     
("OG_SZ", "Klima Schlafzimmer", 10, 30),
("SA",    "Klima Sauna", 30, 90),
]


text_items = [
("og_west",	       "revision_og_west"         ),
("og_ost", 	       "revision_og_ost"          ),
("eg_west",	       "revision_eg_west"         ),
("eg_ost", 	       "revision_eg_ost"          ),
("ug",     	       "revision_ug"              ),
("ch",     	       "revision_ch"              ),
("sauna",     	   "revision_sauna"           ),
("garage", 	       "revision_garage"          ),
("relay_service",  "revision_relay_service"   ),
("bridge_service", "revision_bridge_service"  ),

]




if __name__ == "__main__":

    with open("mqtt.yaml", "w", encoding="utf-8") as fh:

      
        
        fh.write("\n\n- light:\n")
        for item in light_items:
            fh.write(l_format(item[0],item[1]))

        fh.write("\n\n- siren:\n")
        for item in siren_items:
            fh.write(siren_format(item[0],item[1]))
           
        fh.write("\n\n- valve:\n")
        for item in valve_items:
            fh.write(valve_format(item[0], item[1]))
        
        fh.write("\n\n- cover:\n")
        for item in ro_items:
            fh.write(ro_format(item[0], item[1]))

        fh.write("\n\n- sensor:\n")
        for item in sensor_items:
            if item[0].startswith("TI_"):
                fh.write(sensor_format(item[0], item[1], "°C", 0.1, "temperature"))
            elif item[0].startswith("HI_"):
                fh.write(sensor_format(item[0], item[1], "%", 1, "humidity"))
            else:
                fh.write(sensor_format(item[0], item[1], "", 1, "power"))
        
        fh.write("\n\n- climate:\n")
        for item in thermos_items:
            fh.write(thermos_format(item[0], item[1], item[2], item[3]))

        fh.write("\n\n- fan:\n")
        for item in fan_items:
            fh.write(fan_format(item[0], item[1]))

        fh.write("\n\n- text:\n")
        for item in text_items:
            fh.write(text_format(item[0], item[1]))

        fh.write("\n\n- number:\n")
        for item in number_items:
            fh.write(number_format(item[0], item[1]))