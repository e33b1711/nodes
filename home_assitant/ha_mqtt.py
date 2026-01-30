

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

def switch_format(id, friendly_name):  
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
    state_topic: "ard_state/{id}"'''
    str2 = '''
    value_template: >
      {% if value | lower == 'nan' %}
        {{ None }}
      {% else %}
        {{ value }}
      {% endif %}'''
    str3 = f'''
    suggested_display_precision: {precision}
    device_class: "{device_class}"
    unit_of_measurement: "{unit}"
    '''
    return str + str2 + str3

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
]

siren_items = [
('BELL',        "Klingel (dauer)"), 
('ZE_BELL',     "Klingel"), 
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
]  


sensor_items = [
("TI_PU_O", "Temperatur Puffer oben", ""),
("TI_PU_U", "Temperatur Puffer unten", ""),
("F_HE",     "Gastherme", ""),
("F_WW",     "Warnung Pumpensumpf", ""),
("F_RAIN",   "Regen", ""),
("TI_EG_WZ", "Temperatur Wohnzimmer", ""),
("TI_EG_KU", "Temperatur Küche", ""),
("TI_EG_EZ", "Temperatur Esszimmer", ""),
("TI_EG_GA", "Temperatur Gang EG", ""),
("TI_OG_BA", "Temperatur Badezimmer", ""),
("TI_OG_KN", "Temperatur Leo", ""),
("TI_OG_KS", "Temperatur Bini", ""),
("TI_OG_SZ", "Temperatur Schlafzimmer", ""),
("TI_OG_GA", "Temperatur Gang OG", ""),
("TI_UG_HO", "Temperatur Hobby", ""),
("TI_UG_WK", "Temperatur Waschküche", ""),
("TI_UG_HK", "Temperatur Technik", ""),
("TI_UG_GA", "Temperatur Gang UG", ""),
("TI_UG_LA", "Temperatur Lager", ""),
("TI_GR",    "Temperatur Garage", ""),
("TI_AU",    "Temperatur Außen", ""),
("HI_EG_WZ", "Feuchte Wohnzimmer", ""),
("HI_EG_KU", "Feuchte Küche", ""),
("HI_EG_EZ", "Feuchte Esszimmer", ""),
("HI_EG_GA", "Feuchte Gang EG", ""),
("HI_OG_BA", "Feuchte Badezimmer", ""),
("HI_OG_KN", "Feuchte Leo", ""),
("HI_OG_KS", "Feuchte Bini", ""),
("HI_OG_SZ", "Feuchte Schlafzimmer", ""),
("HI_OG_GA", "Feuchte Gang OG", ""),
("HI_UG_HO", "Feuchte Hobby", ""),
("HI_UG_WK", "Feuchte Waschküche", ""),
("HI_UG_HK", "Feuchte Technik", ""),
("HI_UG_GA", "Feuchte Gang UG", ""),
("HI_UG_LA", "Feuchte Lager", ""),
("HI_GR",    "Feuchte Garage", ""),
("HI_AU",    "Feuchte Außen", ""),
]

number_items = [
("U_EL",     "Elektrische Heizung", ""),
]

switch_items = [
("V_OG_KS",  "Heizventil Bini", ""),
("V_OG_KN",  "Heizventil Leo", ""),
("V_OG_GA",  "Heizventil Gang OG", ""),
("V_OG_BA",  "Heizventil Badezimmer", ""),
("V_OG_SZ",  "Heizventil Schlafzimmer", ""),
("V_UG_HO",  "Heizventil Hobby", ""),
("V_UG_H2",  "Heizventil Hobby", ""),
("V_UG_LA",  "Heizventil Lager", ""),
("V_UG_GA",  "Heizventil Gang UG", ""),
("V_UG_WK",  "Heizventil Waschküche", ""),
("V_EG_KU",  "Heizventil Küche", ""),
("V_EG_EZ",  "Heizventil Esszimmer", ""),
("V_EG_E2",  "Heizventil Esszimmer", ""),
("V_EG_GA",  "Heizventil Gang EG", ""),
("V_EG_WZ",  "Heizventil Wohnzimmer", ""),
("V_EG_GR",  "Heizventil Gardarobe", ""),
("V_EG_WC",  "Heizventil WC", ""),
]


text_items = [
("og_west",	       "revision_og_west"         ),
("og_ost", 	       "revision_og_ost"          ),
("eg_west",	       "revision_eg_west"         ),
("eg_ost", 	       "revision_eg_ost"          ),
("ug",     	       "revision_ug"              ),
("gr", 	           "revision_garage"          ),
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

        fh.write("\n\n- text:\n")
        for item in text_items:
            fh.write(text_format(item[0], item[1]))

        fh.write("\n\n- number:\n")
        for item in number_items:
            fh.write(number_format(item[0], item[1]))

        fh.write("\n\n- switch:\n")
        for item in switch_items:
            fh.write(switch_format(item[0], item[1]))