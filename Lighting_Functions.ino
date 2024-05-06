void AUX_Lighting(){         //Sets Lighting Relays to commanded values from Main ECU. 0x91
    if(MAIN == false){
        Sink_Couch = true;
        Liv_Din_Ovh = true;
        Master_Bed = true;
        Compt_Lights = true;
        Bath_Din_Window = true;
        Kit_Ovh_Bath_Sink = true;
    }
    digitalWrite(sink_couch_out,Sink_Couch);
    digitalWrite(ovh_liv_din_out,Liv_Din_Ovh);
    digitalWrite(mast_bed_out,Master_Bed);
    digitalWrite(compt_lamps_out,Compt_Lights);
    digitalWrite(bath_din_out,Bath_Din_Window);
    digitalWrite(kitc_bath_out,Kit_Ovh_Bath_Sink);
}

void Lighting_Control(){
    switch(lighting_cycle){
        case 7:
            Sink_Couch = true;
            Liv_Din_Ovh = true;
            Master_Bed = true;
            Compt_Lights = true;
            Bath_Din_Window = true;
            Kit_Ovh_Bath_Sink = true;
            break;
            
        case 6:
            Sink_Couch = true;
            Liv_Din_Ovh = true;
            Master_Bed = true;
            Compt_Lights = true;
            Bath_Din_Window = true;
            Kit_Ovh_Bath_Sink = false;
            break;
            
        case 4:
            Sink_Couch = true;
            Liv_Din_Ovh = true;
            Master_Bed = true;
            Compt_Lights = true;
            Bath_Din_Window = false;
            Kit_Ovh_Bath_Sink = false;
            break;
            
        case 3:
            Sink_Couch = true;
            Liv_Din_Ovh = true;
            Master_Bed = true;
            Compt_Lights = false;
            Bath_Din_Window = false;
            Kit_Ovh_Bath_Sink = false;
            break;
            
        case 2:
            Sink_Couch = true;
            Liv_Din_Ovh = true;
            Master_Bed = false;
            Compt_Lights = false;
            Bath_Din_Window = false;
            Kit_Ovh_Bath_Sink = false;
            break;

        case 1:
            Sink_Couch = true;
            Liv_Din_Ovh = false;
            Master_Bed = false;
            Compt_Lights = false;
            Bath_Din_Window = false;
            Kit_Ovh_Bath_Sink = false;
            break;
            
        case 0:
            Sink_Couch = false;
            Liv_Din_Ovh = false;
            Master_Bed = false;
            Compt_Lights = false;
            Bath_Din_Window = false;
            Kit_Ovh_Bath_Sink = false;
            break;
        }
    if(lighting_cycle == 7){
        lighting_cycle = 0;
    }
    else{
        lighting_cycle = lighting_cycle + 1;
    }
}