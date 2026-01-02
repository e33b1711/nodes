

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
    state_topic: "ard_state/{id}"
    payload_open: "0"
    payload_close: "100"
    payload_stop: "50"
    state_open: "0"
    state_stopped: "50"
    state_closed: "100"
    optimistic: false
    '''
    return str



def sensor_format(id, friendly_name, unit = "°C", precision = 0.5, device_class = "temperature"):  
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

def number_format(item, unit = "", min = 0, max = 255, device_class = "temperature"):  
    str = f'''
  - unique_id: "{item}"
    name: "{item}"
    command_topic: "ard_command/{item}"
    state_topic: "ard_state/{item}"
    device_class: "{device_class}"
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
    optimistic: false
    '''
    return str

# TODO thermos on off as seperate topic
#TODO rollos stop => node side (more state updates)
#TODO revision strings
#TODO unfy U_ + V_ as number items  U_EL missing!!
##TODO bell must report off



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
('LI_GR_L1',    "Garag L1"),
('ZE_GR_0',	    "Garage Timer"),
('ZE_GR_1',	    "Garage Timer l1"),
('ZE_GR_2',	    "Garage Timer l2"),
('LI_CH',       "Hühnerhaus"),
('LI_CH_L2',    "Hühnerhaus l2"),
('LI_CH_L3',    "Hühnerhaus l3"),
('LI_CH_L4',    "Hühnerhaus l4"),
]

siren_items = [
('BELL',        "Klingel"), 
]



valve_items=[
('PUMP', "Zisterne"), 
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
("V_UG_HO",  "Hobby", ""),
("V_UG_LA",  "Lager", ""),
("V_UG_GA",  "Gang UG", ""),
("V_UG_WK",  "Waschküche", ""),
("V_EG_KU",  "Küche", ""),
("V_EG_EZ",  "Esszimmer", ""),
("V_EG_GA",  "Gang EG", ""),
("V_EG_WZ",  "Woghnzimmer", ""),
("V_EG_GR",  "Gardarobe", ""),
("V_EG_WC",  "WC", ""),
("V_OG_KS",  "Bini", ""),
("V_OG_KN",  "Leo", ""),
("V_OG_GA",  "Gang OG", ""),
("V_OG_BA",  "Badezimmer", ""),
("V_OG_SZ",  "Schlafzimmer", ""),
]



thermos_items = [
("UG_HO", "Klima Hobby"),     
("UG_LA", "Klima Lager"),     
("UG_GA", "Klima Gang UG"),     
("UG_WK", "Klima Waschküche"),     
("EG_KU", "Klima Küche"),     
("EG_EZ", "Klima Esszimmer"),     
("EG_GA", "Klima Gangg EG"),     
("EG_WZ", "Klima Wohnzimmer"),        
("OG_KS", "Klima Bini"),     
("OG_KN", "Klima Leo"),     
("OG_GA", "Klima Gang OG"),     
("OG_BA", "Klima Bad"),     
("OG_SZ", "Klima Schlafzimmer"),
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
                fh.write(sensor_format(item[0], item[1], "°C", 0.5, "temperature"))
            elif item[0].startswith("HI_"):
                fh.write(sensor_format(item[0], item[1], "%", 1, "humidity"))
            else:
                fh.write(sensor_format(item[0], item[1], "", 1, "power"))
        
        fh.write("\n\n- climate:\n")
        for item in thermos_items:
            fh.write(thermos_format(item[0], item[1]))
