

def l_format(item):  
    str = f'''  
  - unique_id: "{item}"
    name: "{item}"
    state_topic: "ard_state/{item}"
    command_topic: "ard_command/{item}"
    payload_on: "1"
    payload_off: "0"
    optimistic: false
    '''
    return str

def ro_format(item):  
    str = f'''
  - unique_id: "{item}"
    name: "{item}"
    command_topic: "ard_command/{item}"
    state_topic: "ard_state/{item}"
    payload_open: "0"
    payload_close: "100"
    payload_stop: "50"
    state_open: "0"
    state_stopped: "50"
    state_closed: "100"
    optimistic: false
    '''
    return str

#TODO rollos stop => node side (more state updates)
#TI / HI V TS revision

def sensor_format(item, unit = "°C", precision = 0.5, device_class = "temperature"):  
    str = f'''
  - unique_id: "{item}"
    state_class: "measurement"
    name: "{item}"
    state_topic: "ard_state/{item}"
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


def thermos_format(ts_item, ti_item, hi_item, min = 10, max = 30):
    str = f'''
  - unique_id: "{ts_item}"
    name: "{ts_item}"
    modes:
      - "off"
      - "heat"
    temperature_command_topic: "ard_command/{ts_item}"
    temperature_state_topic: "ard_state/{ts_item}"
    current_humidity_topic: "ard_state/{hi_item}"
    current_temperature_topic: "ard_state/{ti_item}"
    mode_command_topic: "ard_command/{ts_item}"
    mode_state_topic: "ard_state/{ts_item}"
    min_temp: "{min}"
    max_temp: "{max}"
    optimistic: false
    '''
    return str

# TODO thermos on off as seperate topic



light_items = [
'LI_EG_WZ',	 
'LI_EG_WZ_L1',
'LI_EG_WZ_L2',
'LI_EG_KU_L1',
'LI_EG_EZ', 	
'LI_EG_EZ_L1',
'LI_EG_EZ_L3',
'LI_EG_EZ_KU',
'LI_EG_GA',  
'LI_GA_L1',	
'LI_EG_GR',  
'LI_EG_WC',	 
'LI_EG_SP',	
'LI_OG_BA',  
'LI_OG_KN',	 
'LI_OG_KN_L1',
'LI_OG_KS', 	 
'LI_OG_KS_L1',
'LI_OG_SZ', 	 
'LI_OG_SZ_L1',
'LI_OG_SZ_L2',
'LI_OG_GA', 	 
'LI_OG_GA_L1',
'LI_UG_HO', 	 
'LI_UG_WK',   
'LI_UG_HK',   
'LI_UG_GA',   
'LI_UG_HN',   
'LI_UG_TR',   
'LI_EG_VH',   
'ZE_EG_VH',   
'LI_EG_AS',   
'LI_EG_AW',   
'LI_EG_AO',   
'LI_GR',  
'LI_GR_L1',   
'ZE_GR_0',	   
'ZE_GR_1',	   
'ZE_GR_2',	   
'LI_CH',  
'LI_CH_L2',   
'LI_CH_L3',   
'LI_CH_L4',   
'BELL',   
'PUMP']  

ro_items = [
"RO_EG_SU",  
"RO_EG_WE",  
"RO_OG_BA",  
"DF_OG_GA",  
"VD_OG_GA",  
"RO_OG_KN",  
"VD_OG_KN",  
"DF_OG_KN",  
"RO_OG_KS",  
"VD_OG_KS",  
"DF_OG_KS",  
"RO_OG_SZ",  
"VD_OG_SZ",  
"DF_OG_SZ",  
"DO_GR",  
"DO_CH"]


sensor_items = [
"TI_PU_O ",
"TI_PU_U ",
"F_HE",
"F_WW",
"F_RAIN",
"V_UG_HO",
"V_UG_LA",
"V_UG_GA",
"V_UG_WK",
"V_EG_KU",
"V_EG_EZ",
"V_EG_GA",
"V_EG_WZ",
"V_EG_GR",
"V_EG_WC",
"V_OG_KS",
"V_OG_KN",
"V_OG_GA",
"V_OG_BA",
"V_OG_SZ",
]

#TODO unfy U_ + V_ as number items

thermos_items = [
"GR",
"AU",
"CH",
"UG_HO",     
"UG_LA",     
"UG_GA",     
"UG_WK",     
"EG_KU",     
"EG_EZ",     
"EG_GA",     
"EG_WZ",     
"EG_GR",     
"EG_WC",     
"OG_KS",     
"OG_KN",     
"OG_GA",     
"OG_BA",     
"OG_SZ",
]





if __name__ == "__main__":

    with open("mqtt.yaml", "w", encoding="utf-8") as fh:
        fh.write("- light:\n")
        for item in light_items:
            fh.write(l_format(item))
        
        fh.write("\n\n- cover:\n")
        for item in ro_items:
            fh.write(ro_format(item))

        fh.write("\n\n- sensor:\n")
        for item in sensor_items:
            if item.startswith("TI_"):
                fh.write(sensor_format(item, "°C", 0.5, "temperature"))
            elif item.startswith("HI_"):
                fh.write(sensor_format(item, "%", 1, "humidity"))
            else:
                fh.write(sensor_format(item, "", 1, "power"))
        
        fh.write("\n\n- climate:\n")
        for item in thermos_items:
            fh.write(thermos_format("TS_" + item, "TI_" + item, "HI_" + item))
