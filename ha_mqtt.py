

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





if __name__ == "__main__":

    with open("mqtt.yaml", "w") as fh:
        fh.write("- light:\n")
        for item in light_items:
            fh.write(l_format(item))
        
        fh.write("\n\n- cover:\n")
        for item in ro_items:
            fh.write(ro_format(item))



#- sensor:
#  - unique_id: "TI_OG_GA"
#    name: "TI_OG_GA"
#    state_topic: "ard_state/TI_OG_GA"
#    suggested_display_precision: 1
#    unit_of_measurement: "°C"
#
#  - unique_id: "HI_OG_GA"
#    name: "HI_OG_GA"
#    state_topic: "ard_state/HI_OG_GA"
#    unit_of_measurement: "%"
#
#- number:
#  - unique_id: "TS_OG_GA"
#    name: "TS_OG_GA"
#    state_topic: "ard_state/TS_OG_GA"
#    command_topic: "ard_command/TS_OG_GA"
#    unit_of_measurement: "°C"
#    optimistic: false