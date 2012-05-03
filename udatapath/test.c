 /* packing */
    struct ofl_msg_flow_mod msg =
            {{.type = OFPT_FLOW_MOD},
             .cookie = 0x0000000000000000ULL,
             .cookie_mask = 0x0000000000000000ULL,
             .table_id = 0xff,
             .command = OFPFC_ADD,
             .idle_timeout = 0,
             .hard_timeout = 0,
             .priority = 0x8000,
             .buffer_id = 0xffffffff,
             .out_port = OFPP_ANY,
             .out_group = OFPG_ANY,
             .flags = 0x0000,
             .match = NULL,
             .instructions_num = 0,
             .instructions = NULL};
     struct ofl_match m;
     ofl_structs_match_init(&m);
     ofl_structs_match_put8(&m, OXM_OF_IP_PROTO, 6);
     ofl_structs_match_put16(&m, OXM_OF_ETH_TYPE, 0x800);
     ofl_structs_match_put16(&m,OXM_OF_TCP_SRC,80);    
     ofl_structs_match_put32(&m, OXM_OF_IN_PORT, 1);
     msg.match = (struct ofl_match_header *) &m;
     
     msg.instructions = xmalloc(sizeof(struct ofl_instruction_header *) * 1);
     struct ofl_instruction_actions *i = xmalloc(sizeof(struct ofl_instruction_actions));
     i->header.type = OFPIT_WRITE_ACTIONS; 
     i->actions = xmalloc(sizeof(struct ofl_action_header*));
     i->actions_num = 0;
     
     struct ofl_action_header *act = NULL;
     struct ofl_action_output *a = xmalloc(sizeof(struct ofl_action_output));
     a->port = 1;
     a->header.type = OFPAT_OUTPUT;
     act = (struct ofl_action_header *)a;
       
     i->actions_num++;
     msg.instructions_num++;
     *i->actions = act;
     *msg.instructions = (struct ofl_instruction_header *)i;
     uint8_t *buf;
     size_t len;
     ofl_msg_pack(&msg.header,0xf0ff00f0, &buf, &len, NULL);
    
     /* UNPACK */
     uint32_t xid;
     struct ofl_msg_header *msga;
     int error = ofl_msg_unpack(buf,len,&msga,&xid, NULL);
     if (error)
        printf("error %d \n", ((error) & 0xffff));          
     else {
         struct ofl_msg_flow_mod *f = (struct ofl_msg_flow_mod *) msga;
         struct ofl_match *mt = (struct ofl_match*) f->match;
         struct ofl_match_tlv *oft;
         HMAP_FOR_EACH_WITH_HASH(oft, struct ofl_match_tlv, hmap_node, hash_int(OXM_OF_ETH_TYPE, 0),
              &mt->match_fields) {
            uint16_t value;
            memcpy(&value, oft->value,sizeof(uint16_t));
            printf("VALUE %x\n", value);
         ofl_msg_free_flow_mod(f,true,false,NULL);
        }
    }
     
    
     
     /* Retrieving
     uint8_t *t = buf + 52;
     uint32_t header, port;
     memcpy(&header,t,4);
     memcpy(&port,t + 4,4);
      printf("OXM IN_PORT %x %d %d\n", OXM_VENDOR(htonl(header)), OXM_LENGTH(htonl(header)), port); 
     t += 8;
     uint16_t dl;
     memcpy(&header,t,4);
     memcpy(&dl,t + 4,2);
     printf("OXM DL_TYPE %x %d %x\n", OXM_VENDOR(htonl(header)), OXM_LENGTH(htonl(header)), dl);  
     t += 6;    
     uint16_t tcp;
     memcpy(&header,t,4);
     memcpy(&tcp,t + 4,2);
     printf("OXM TCP %x %d %d\n", OXM_VENDOR(htonl(header)), OXM_LENGTH(htonl(header)), tcp);             
     struct ofp_flow_mod *mod = (struct ofp_flow_mod * )buf;
     printf("MATCH %d %d\n", ntohs(mod->match.type), ntohs(mod->match.length));*/
