// copy to include/secrets.h (ignored by git) and set the door codes for the garage keypad
const int num_codes = 2;
const String code[num_codes] = {"0000", "1111"};
// front door: 10 s countdown on the keypad, then home assistant opens the nuki
const int num_door_codes = 1;
const String door_code[num_door_codes] = {"2222"};
