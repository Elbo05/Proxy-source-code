#include "events.h"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include "dialog.h"
#include <thread>
#include <limits.h>
#include <future>
bool fastvend = false;
bool fastdrop = false;
bool fasttrash = false;
bool vendtype = false;
bool passforce = false;
bool banscam = false;
bool autorespull = false;
bool showcoord = false;
int amountvend = 1;
bool worldbanjoinmod = false;
int posx = 0;
int posy = 0;
int iditemm = 2;
bool automsg = false;

int wrenchmode = 0;
bool wrenchpull = false;
bool wrenchkick = false;
bool wrenchban = false;
bool wrenchtrade = false;
bool wrenchadd = false;
bool wrenchtp = false;
bool wrenchmodeon = false;
bool fakeblink = false;


int pwd = 0;





int ncx = 1000;
int ncy = 200;
int ncpx = 500;
int ncpy = 0;
int ncxs = 2000;
int ncys = 500;
int nccs = 0;




std::string mactutu = "02:00:00:00:00:00";


std::string totpull = "`4Disable";
std::string totkick = "`4Disable";
std::string totban = "`4Disable";
std::string tottrade = "`4Disable";
std::string totadd = "`4Disable";
std::string tottp = "`4Disable";

std::string tutitut = "0";
std::string messagelolos = "";

/*
bool followply = false;
std::string namesusi = "";
*/


bool events::out::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    varlist.serialize_from_mem(utils::get_extended(packet));
    PRINTS("varlist: %s\n", varlist.print().c_str());
    return false;
}

bool events::out::pingreply(gameupdatepacket_t* packet) {
    //since this is a pointer we do not need to copy memory manually again
    packet->m_vec2_x = 1000.f;  //gravity
    packet->m_vec2_y = 250.f;   //move speed
    packet->m_vec_x = 64.f;     //punch range
    packet->m_vec_y = 64.f;     //build range
    packet->m_jump_amount = 0;  //for example unlim jumps set it to high which causes ban
    packet->m_player_flags = 0; //effect flags. good to have as 0 if using mod noclip, or etc.
    return false;
}

bool find_command(std::string chat, std::string name) {
    bool found = chat.find("/" + name) == 0;
    if (found)
        gt::send_log("`6" + chat);
    return found;
}


bool events::out::generictext(std::string packet) {
    PRINTS("Generic text: %s\n", packet.c_str());

    

    auto& world = g_server->m_world;
    rtvar var = rtvar::parse(packet);
    if (!var.valid())
        return false;

    if (packet.find("buttonClicked|bypass_safevault") != -1) {
        int vaultx = std::stoi(packet.substr(packet.find("tilex|") + 6, packet.length() - packet.find("tilex|") - 1));
        int vaulty = std::stoi(packet.substr(packet.find("tiley|") + 6, packet.length() - packet.find("tiley|") - 1));

        posx = vaultx;
        posy = vaulty;
        Dialog bpsv;
        bpsv.addLabelWithIcon("Bypass Option", 8878, LABEL_BIG);
        bpsv.addSpacer(SPACER_SMALL);
        bpsv.addSmallText("Vault Pos X : " + std::to_string(vaultx));
        bpsv.addSmallText("Vault Pos Y : " + std::to_string(vaulty));
        bpsv.endDialog("options_menu_v", "Apply", "Cancel");
        gt::send_log("`9Pos X, Pos Y applied!. Please write /vault");
        return false;
    }


    if (packet.find("worldbanmod|") != -1) {
        try
        {
            std::string aaa = packet.substr(packet.find("mod|") + 4, packet.size());
            std::string number = aaa.c_str();
            worldbanjoinmod = stoi(number);
            if (worldbanjoinmod == true)
            {
                gt::send_log("`9Successfuly started checking entered for `#@Moderators `9and `#@Guardians`9, please wait...");
            }
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `2override detected");

        }
    }

    if (packet.find("vendudu") != -1) {
        int vend_1 = std::stoi(packet.substr(packet.find("vend_1|") + 7, packet.length() - packet.find("vend_1|") - 1));
        try {
            int vend_2 = std::stoi(packet.substr(packet.find("vend_2|") + 7, packet.length() - packet.find("vend_2|") - 1));
            amountvend = vend_2;
        } catch (std::exception) { gt::send_log("Critical Error : Input Number Only"); }
        int vend_3 = std::stoi(packet.substr(packet.find("vend_3|") + 7, packet.length() - packet.find("vend_3|") - 1));
        int vend_4 = std::stoi(packet.substr(packet.find("vend_4|") + 7, packet.length() - packet.find("vend_4|") - 1));
        if (vend_1 > 0) {
            fastvend = true;
        } else {
            fastvend = false;
        }
        if (vend_3 > 0) {
            vendtype = true;
        } else {
            vendtype = false;
        }
        if (vend_4 > 0) {
            banscam = true;
        } else {
            banscam = false;
        }
        return true;
    } else if (packet.find("autotomsgg_kasaak") != -1) {
        try {
            int msg_1 = std::stoi(packet.substr(packet.find("message_1|") + 10, packet.length() - packet.find("message_1|") - 1));
            std::string msg_2 = packet.substr(packet.find("message_2|") + 10, packet.length() - packet.find("message_2|") - 1);
            std::string delimeter = " "; 
            messagelolos = delimeter + msg_2;
            gt::send_log("Message : " + messagelolos);
            if (msg_1 > 0) {
                automsg = true;
            } else {
                automsg = false;
            }
        } catch (std::exception) { gt::send_log("Critical Error : Something Error. Try Again Later"); }
        return true;
    } else if (packet.find("options_menu_v") != -1) {
        int menu_1 = std::stoi(packet.substr(packet.find("fastdrop_change|") + 16, packet.length() - packet.find("fastdrop_change|") - 1));
        int menu_2 = std::stoi(packet.substr(packet.find("fasttrash_change|") + 17, packet.length() - packet.find("fasttrash_change|") - 1));
        int menu_3 = std::stoi(packet.substr(packet.find("vendtype_change|") + 16, packet.length() - packet.find("vendtype_change|") - 1));
        int menu_4 = std::stoi(packet.substr(packet.find("banplayer_change|") + 17, packet.length() - packet.find("banplayer_change|") - 1));
        int menu_5 = std::stoi(packet.substr(packet.find("fastvend_change|") + 16, packet.length() - packet.find("fastvend_change|") - 1));
        
        int menu_7 = std::stoi(packet.substr(packet.find("passforce_change|") + 17, packet.length() - packet.find("passforce_change|") - 1));
        
        int menu_9 = std::stoi(packet.substr(packet.find("showcoord_change|") + 17, packet.length() - packet.find("showcoord_change|") - 1));
        

        

        if (menu_1 > 0) {//
            fastdrop = true;
        } else {
            fastdrop = false;
        }
        if (menu_2 > 0) {//
            fasttrash = true;
        } else {
            fasttrash= false;
        }
        if (menu_3 > 0) {//
            vendtype = true;
        } else {
            vendtype = false;
        }
        if (menu_4 > 0) {
            banscam = true;
        } else {
            banscam = false;
        }
        if (menu_5 > 0) {//
            fastvend = true;
        } else {
            fastvend = false;
        }
        if (menu_7 > 0) {//
            passforce = true;
        } else {
            passforce = false;
        }
        if (menu_9 > 0) {
            showcoord = true;
        } else {
            showcoord = false;
        }
        try {
            int menu_6 = std::stoi(packet.substr(packet.find("vendamount_change|") + 18, packet.length() - packet.find("vendamount_change|") - 1));
            int menu_8 = std::stoi(packet.substr(packet.find("passfstart_change|") + 18, packet.length() - packet.find("passfstart_change|") - 1));
            amountvend = menu_6;
            pwd = menu_8;
        } catch (std::exception) { gt::send_log("Critical Error : Input Number Only!"); }
        return true;
    } else if (packet.find("wrenchmode_change|") != -1) {
        try {
            int menu_11 = std::stoi(packet.substr(packet.find("wrenchmode_change|") + 18, packet.length() - packet.find("wrenchmode_change|") - 1));
            wrenchmode = menu_11;
            if (menu_11 > 6) {
                gt::send_log("Critical Error : Input Number 0-6 Only!");
                wrenchmodeon = false;
                menu_11 = 0;
                wrenchmode = 0;
            } else if (menu_11 < 0) {
                gt::send_log("Critical Error : Input Number 0-6 Only!");
                wrenchmodeon = false;
                menu_11 = 0;
                wrenchmode = 0;
            } else if (menu_11 == 0) {
                wrenchmodeon = false;
                wrenchpull = false;
                wrenchkick = false;
                wrenchban = false;
                wrenchtrade = false;
                wrenchadd = false;
                wrenchtp = false;
            } else if (menu_11 == 1) {
                wrenchmodeon = true;
                wrenchpull = true;
                wrenchkick = false;
                wrenchban = false;
                wrenchtrade = false;
                wrenchadd = false;
                wrenchtp = false;
            } else if (menu_11 == 2) {
                wrenchmodeon = true;
                wrenchpull = false;
                wrenchkick = true;
                wrenchban = false;
                wrenchtrade = false;
                wrenchadd = false;
                wrenchtp = false;
            } else if (menu_11 == 3) {
                wrenchmodeon = true;
                wrenchpull = false;
                wrenchkick = false;
                wrenchban = true;
                wrenchtrade = false;
                wrenchadd = false;
                wrenchtp = false;
            } else if (menu_11 == 4) {
                wrenchmodeon = true;
                wrenchpull = false;
                wrenchkick = false;
                wrenchban = false;
                wrenchtrade = true;
                wrenchadd = false;
                wrenchtp = false;
            } else if (menu_11 == 5) {
                wrenchmodeon = true;
                wrenchpull = false;
                wrenchkick = false;
                wrenchban = false;
                wrenchtrade = false;
                wrenchadd = true;
                wrenchtp = false;
            } else if (menu_11 == 6) {
                wrenchmodeon = false;
                wrenchpull = false;
                wrenchkick = false;
                wrenchban = false;
                wrenchtrade = false;
                wrenchadd = false;
                wrenchtp = true;
            }
        } catch (std::exception) { gt::send_log("Critical Error : Input Number Only!"); }
        return true;
    } else if (wrenchmodeon == true) {
        if (packet.find("action|wrench") != -1) {
            g_server->send(false, packet);
            int nettidd = std::stoi(packet.substr(packet.find("netid|") + 6, packet.length() - packet.find("netid|") - 1));
            if (wrenchpull == true) {
                g_server->send(
                    false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(nettidd) + "|\nnetID|" + std::to_string(nettidd) + "|\nbuttonClicked|pull");
            } else if (wrenchkick == true) {
                g_server->send(
                    false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(nettidd) + "|\nnetID|" + std::to_string(nettidd) + "|\nbuttonClicked|kick");
            } else if (wrenchban == true) {
                g_server->send(false,
                    "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(nettidd) + "|\nnetID|" + std::to_string(nettidd) + "|\nbuttonClicked|worldban");
            } else if (wrenchtrade == true) {
                g_server->send(false,
                    "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(nettidd) + "|\nnetID|" + std::to_string(nettidd) + "|\nbuttonClicked|trade");
            } else if (wrenchadd == true) {
                g_server->send(false,
                    "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(nettidd) + "|\nnetID|" + std::to_string(nettidd) + "|\nbuttonClicked|friend_add");
            }



            return true;
        }
    } else if (packet.find("vendamount_change|") != -1) {
        try {
            int menu_6 = std::stoi(packet.substr(packet.find("vendamount_change|") + 18, packet.length() - packet.find("vendamount_change|") - 1));
            amountvend = menu_6;
        } catch (std::exception) { gt::send_log("Critical Error : Input Number Only!"); }
        return true;
    } else if (packet.find("passfstart_change|") != -1) {
        try {
            int menu_8 = std::stoi(packet.substr(packet.find("passfstart_change|") + 18, packet.length() - packet.find("passfstart_change|") - 1));
            pwd = menu_8;
        } catch (std::exception) { gt::send_log("Critical Error : Input Number Only!"); }
        return true;
    }


    //vault bypass
    if (packet.find("xvault111|") != -1) {
        std::string aaa = packet.substr(packet.find("111|") + 4, packet.size());
        try
        {
            posx = std::stoi(aaa.c_str());
            gt::send_log("posx: " + std::to_string(posx));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");

        }
    }
   
    
    if (packet.find("2vault222|") != -1) {
        std::string aaa = packet.substr(packet.find("222|") + 4, packet.size());
        while (!aaa.empty() && isspace(aaa[aaa.size() - 1]))
            aaa.erase(aaa.end() - (76 - 0x4B));
        try
        {
            posy = std::stoi(aaa.c_str());
            gt::send_log("posy: " + std::to_string(posy));
        }
        catch (exception a)
        {
            gt::send_log("`4Critical Error: `9Please enter numbers only.");
        }
    }


































    /*std::string mactod = "mac|";
    std::size_t mactut = packet.find(mactod);

    if (mactut != std::string::npos) {
        packet.replace(packet.find(mactod) + 4, 17, mactutu);
        return true;
    }*/

    if (var.get(0).m_key == "action" && var.get(0).m_value == "input") {
        if (var.size() < 2)
            return false;
        if (var.get(1).m_values.size() < 2)
            return false;

        if (!world.connected)
            return false;

        auto chat = var.get(1).m_values[1];

        if (find_command(chat, "name ")) { //ghetto solution, but too lazy to make a framework for commands.
            std::string name = "``" + chat.substr(6) + "``";
            variantlist_t va{ "OnNameChanged" };
            va[1] = name;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("`9Name Set To: " + name);
            return true;
             } else if (find_command(chat, "dropwl ")) {
            std::string cdropcount = chat.substr(8);
            g_server->send(false, "action|drop\n|itemID|242");
            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|242|\ncount|" + cdropcount); // 242 is id of World Lock
            gt::send_log("`9Dropped `c" + cdropcount + "`9 wl");
            return true;
              } else if (find_command(chat, "dropdl ")) {
            std::string cdropcounts = chat.substr(8);
            g_server->send(false, "action|drop\n|itemID|1796");
            g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|1796|\ncount|" + cdropcounts); // 1796 is id of World Lock
            gt::send_log("`9Dropped `c" + cdropcounts + "`9 wl");
            return true;
            
            
            
            
            
        } /*else if (find_command(chat, "follow ")) {
            auto& players = g_server->m_world.players;
            for (auto& player : players) {
                std::string supri = chat.substr(8);
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(supri) == 0) {
                    namesusi = name_2;
                    followply = true;
                }
            }
            return true;
        } else if (find_command(chat, "unfollow")) {
            namesusi = "";
            followply = false;
        } */
        else if (find_command(chat, "vault")) {
            Dialog c;
            c.addTextBox("`#[SahyuiMOD] `w: Whitelisted Player List");
            std::string paket;
            paket =
                "add_label_with_icon|big|Bypass Vault Safe|left|2|\nadd_spacer|small\n"
                "\nadd_spacer|small"
                "\nadd_text_input|xvault111|PosX|" + std::to_string(posx) + "|3|"
                "\nadd_text_input|2vault222|PosY|" + std::to_string(posy) + "|3|"
                "\nadd_text_input|iditemzz1|ItemID|" + std::to_string(iditemm) + "|6|"
                "\nadd_spacer|small"
                "\nend_dialog|end|Cancel|Set|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "bypassvault")) {
            g_server->send(false, "action|dialog_return\ndialog_name|storageboxxtreme\ntilex|" + std::to_string(posx) + "|\ntiley|" + std::to_string(posy) + "|\nitemid|" + std::to_string(iditemm) + "|\nbuttonClicked|do_add\nitemcount|1");
            return true;
        }








        else if (find_command(chat, "flag ")) {
            int flag = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnGuildDataChanged" };
            va[1] = 1;
            va[2] = 2;
            va[3] = flag;
            va[4] = 3;
            g_server->send(true, va, world.local.netid, -1);
            gt::send_log("Flag Set To Item Id: `c" + std::to_string(flag));
            return true;
        }
        else if (find_command(chat, "ghost")) {
            gt::ghost = !gt::ghost;
            if (gt::ghost)
                gt::send_log("`9Ghost Enabled");
            else
                gt::send_log("`9Ghost Disabled");
            return true;
        }
        else if (find_command(chat, "devmode")) {
            gt::devmode = !gt::devmode;
            if (gt::devmode)
                gt::send_log("`9Dev Mode Enabled");
            else
                gt::send_log("`9Dev Mode Disabled");
            return true;
        }
        else if (find_command(chat, "fakelag")) {
            fakeblink = !fakeblink;
            if (fakeblink)
                gt::send_log("`9Fake Lag Enabled");
            else
                gt::send_log("`9Fake Lag Disabled");
            return true;
        }
        else if (find_command(chat, "jail ")) {
            int id = std::stoi(chat.substr(6));
            int x = g_server->m_world.local.pos.m_x / 32;
            int y = (g_server->m_world.local.pos.m_y / 32) - 1;
            g_server->addBlock(id, x, y);
            g_server->addBlock(id, x - 1, y + 1);
            g_server->addBlock(id, x + 1, y + 1);
            g_server->addBlock(id, x, y + 2);
            gt::send_log("Spawned With ID : " + std::to_string(id));
            return true;
        }
        else if (find_command(chat, "placetp")) {
            gt::placetp = !gt::placetp;
            if (gt::placetp == true) {
                gt::send_log("Place Teleport On");
            }
            else {
                gt::send_log("Place Teleport Off");
            } return true;
        }
        else if (find_command(chat, "noclip")) {
            gt::noclip = !gt::noclip;
            g_server->sendState(g_server->m_world.local.netid);
            if (gt::noclip)
                gt::send_log("`9NoClip Enabled");
            else
                gt::send_log("`9NoClip Disabled");
            return true;
        }
        else if (find_command(chat, "country ")) {
            std::string cy = chat.substr(9);
            gt::flag = cy;
            gt::send_log("Your Country Set To `c" + cy + "`9, (Relog To Game To Change It Successfully!)");
            return true;
        }
        else if (find_command(chat, "uid ")) {
            std::string name = chat.substr(5);
            gt::send_log("`9Resolving Uid For `2" + name);
            g_server->send(false, "action|input\n|text|/ignore /" + name);
            g_server->send(false, "action|friends");
            gt::resolving_uid2 = true;
            return true;
        }
        else if (find_command(chat, "tp ")) {
            std::string name = chat.substr(4);
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                if (name_2.find(name) == 0) {
                    gt::send_log("`9Teleporting To `2" + player.name);
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                    break;
                }
            }
            return true;
        }
        else if (find_command(chat, "warp ")) {
            std::string name = chat.substr(6);
            gt::send_log("`9Warping To `2" + name);
            g_server->send(false, "action|join_request\nname|" + name, 3);
            return true;
        }
        else if (find_command(chat, "tpid ")) {
            std::string worldname = g_server->m_world.name.c_str();
            std::string idkntl = chat.substr(6);
            g_server->send(false, "action|join_request\nname|" + worldname + "|" + idkntl, 3);
            return true;
        }
        else if (find_command(chat, "pullall")) {
            std::string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(player.netid) + "|\nbuttonClicked|pull");
                    // You Can |kick |trade |worldban
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
            return true;
        }
        else if (find_command(chat, "banall")) {
            std::string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(player.netid) + "|\nbuttonClicked|worldban");
                    // You Can |kick |trade |worldban
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
            return true;
        }
        else if (find_command(chat, "tradeall")) {
            std::string username = chat.substr(6);
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2); //remove color
                if (name_2.find(username)) {
                    g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    g_server->send(false, "action|dialog_return\ndialog_name|popup\nnetID|" + std::to_string(player.netid) + "|\nbuttonClicked|trade");
                    // You Can |kick |trade |worldban
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }
            }
            return true;
        }
        else if (find_command(chat, "skin ")) {
            int skin = atoi(chat.substr(6).c_str());
            variantlist_t va{ "OnChangeSkin" };
            va[1] = skin;
            g_server->send(true, va, world.local.netid, -1);
            return true;
        }
        else if (find_command(chat, "fastvend")) {
            if (fastvend == false) {
                fastvend = true;
                gt::send_log("`9Fast Vend Enable");
            }
            else {
                fastvend = false;
                gt::send_log("`9Fast Vend Disable");
            }
            return true;
        
        } else if (find_command(chat, "fastdrop")) {
            if (fastdrop == false) {
                fastdrop = true;
                gt::send_log("`9Fast Drop Enable");
            } else {
                fastdrop = false;
                gt::send_log("`9Fast Drop Disable");
            }
            return true;
        } else if (find_command(chat, "fasttrash")) {
            if (fasttrash == false) {
                fasttrash = true;
                gt::send_log("`9Fast Trash Enable");
            } else {
                fasttrash = false;
                gt::send_log("`9Fast Trash Disable");
            }
            return true;
        } else if (find_command(chat, "passforce")) {
            if (passforce == false) {
                passforce = true;
                gt::send_log("`9Passforce Enable");
            } else {
                passforce = false;
                gt::send_log("`9Passforce Disable");
                pwd = 0;
            }
            return true;
        } else if (find_command(chat, "vendtype")) {
            if (vendtype == false) {
                vendtype = true;
                gt::send_log("`9Vend Type Enable");
            } else {
                vendtype = false;
                gt::send_log("`9Vend Type Disable");
            }
            return true;
        } else if (find_command(chat, "autorespull")) {
            if (autorespull == false) {
                autorespull = true;
                gt::send_log("`9Auto Respawn When Pulled Enable");
            } else {
                autorespull = false;
                gt::send_log("`9Auto Respawn When Pulled Disable");
            }
            return true;
        } else if (find_command(chat, "wrench ")) {
            std::string name = chat.substr(6);
            std::string username = ".";
            for (auto& player : g_server->m_world.players) {
                auto name_2 = player.name.substr(2);
                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                g_server->send(false, "action|wrench\n|netid|" + std::to_string(player.netid));
            }
            return true;
        }



        else if (find_command(chat, "menu")) {
            Dialog menu;
            menu.addLabelWithIcon("Proxy Menu", 32, LABEL_BIG);
            menu.addCheckbox("fastdrop_change", "Fast Drop", fastdrop);
            menu.addCheckbox("fasttrash_change", "Fast Trash", fasttrash);
            menu.addCheckbox("vendtype_change", "Vend Type", vendtype);
            menu.addCheckbox("banplayer_change", "Auto Ban People If Say 'Scam'", banscam);
            menu.addCheckbox("worldbanmod", "Exit world when mod joins", worldbanjoinmod);
            menu.addCheckbox("fastvend_change", "Fast Vend", fastvend);
            menu.addInputBox("vendamount_change", "Fast Vend Amount Buy", std::to_string(amountvend), 3);
            menu.addSpacer(SPACER_SMALL);
            menu.addCheckbox("passforce_change", "Pass Force", passforce);
            menu.addInputBox("passfstart_change", "Pass Force Start Number", std::to_string(pwd), 10);
            menu.addSpacer(SPACER_SMALL);
            menu.addCheckbox("showcoord_change", "Show Coordinate", showcoord);
            menu.addSpacer(SPACER_SMALL);
            menu.addTextBox("Wrench Mode [P : 1] [K : 2] [B : 3] [T : 4] [A : 5] [TP : 6]");
            menu.addSpacer(SPACER_SMALL);
            menu.addInputBox("wrenchmode_change", "Wrench Mode : ", std::to_string(wrenchmode), 3);
            //menu.addPicker("packetid_change", "Pickup", "Select From Inventory");
            menu.endDialog("options_menu_v", "Apply", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = menu.finishDialog();
            g_server->send(true, liste);
            return true;
        }

        else if (find_command(chat, "proxy")) {
            std::string paket;
            paket =
                "\nadd_label_with_icon|big|Proxy Commands|left|32|"
                "\nadd_spacer|small"
                "\nadd_textbox|`9/proxy `#(Show Commands)|left|2480|"
                "\nadd_textbox|`9/menu `#(Show Menu)|left|2480|"
                "\nadd_textbox|`9/vend `#(Show Vend Hack)|left|2480|"
                "\nadd_textbox|`9/tp [name] `#(Teleport To Target Player) |left|2480|"
                "\nadd_textbox|`9/ghost `#(Ghost Mode)|left|2480|"
                "\nadd_textbox|`9/uid [name] `#(Resolve Target UID)|left|2480|"
                "\nadd_textbox|`9/warp [world] `#(Warp To Other World)|left|2480|"
                "\nadd_textbox|`9/flag [id] `#(Change Flag To Item ID)|left|2480|"
                "\nadd_textbox|`9/skin [id] `#(Change Skin To Skin ID)|left|2480|"
                "\nadd_textbox|`9/country [key] `#(Change Flag To Other Country)|left|2480|"
                "\nadd_textbox|`9/name [name] `#(Change GrowID But Its Visual)|left|2480|"
                "\nadd_textbox|`9/fastdrop `#(Fast Drop With 1 Press)|left|2480|"
                "\nadd_textbox|`9/fasttrash `#(Fast Trash With 1 Press)|left|2480|"
                "\nadd_textbox|`9/fastvend `#(Buy 1 Item From Vend Only With Wrenching Vend)|left|2480|"
                "\nadd_textbox|`9/passforce `#(Auto Input Password)|left|2480|"
                "\nadd_textbox|`9/pullall `#(Pull All Player In Same World)|left|2480|"
                "\nadd_textbox|`9/banall `#(Ban All Player In Same World)|left|2480|"
                "\nadd_textbox|`9/tradeall `#(Trade All Player In Same World)|left|2480|"
                "\nadd_quick_exit|"
                "\nend_dialog|optionlsls|Cancel|Okay|";
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = paket;
            g_server->send(true, liste);
            return true;
        }
        else if (find_command(chat, "server")) {
            std::string babang;
            babang =
                "add_label_with_icon|big|Server Settings|left|32|"
                "\nadd_spacer|small"
                "\nadd_textbox|Server IP : " +
                g_server->m_server +
                "|left|2480|"
                "\nadd_textbox|Server Port : " +
                std::to_string(g_server->m_port) +
                "|left|2480|"
                "\nend_dialog|end|Cancel|Okay|";
            variantlist_t lsals{ "OnDialogRequest" };
            lsals[1] = babang;
            g_server->send(true, lsals);
            return true;

            // /world
        }
        else if (find_command(chat, "world")) {

        std::string paket;
        paket =
            "\nadd_label_with_icon|big|World Commands|left|32|"
            "\nadd_spacer|small"
            "\nadd_button|killall|`4Kill All``|noflags|0|0|"
            "\nadd_button|banall|`4Ban All``|noflags|0|0|"
            "\nadd_button|pullall|`5Pull All``|noflags|0|0|"
            "\nadd_button|ubaworld|`5Unban World``|noflags|0|0|"
            "\nadd_textbox|`4IMPORTANT: `oDiscord: Sahyui#1067 `wDiscord add : `w tinyurl.com/sahyuidc.|left|2480|"
            "\nadd_quick_exit|"
            "\nend_dialog|end|Cancel|Okay|";
        variantlist_t liste{ "OnDialogRequest" };
        liste[1] = paket;
        g_server->send(true, liste);
        return true;

        } else if (find_command(chat, "vend")) {
            Dialog a;
            a.addLabelWithIcon("Vend Option", 2978, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("vend_1", "Fast Vend", fastvend);
            a.addSpacer(SPACER_SMALL);
            a.addInputBox("vend_2", "Fast Vend Amount Buy", std::to_string(amountvend), 3);
            a.addSpacer(SPACER_SMALL);
            a.addCheckbox("vend_3", "Vend Type", vendtype);
            a.addCheckbox("vend_4", "Auto Ban", banscam);
            a.endDialog("vendudu", "Done", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);

            return true;
        } else if (find_command(chat, "wrenchmode")) {
            if (wrenchpull == true) {
                totpull = "`2Enable";
            } else {
                totpull = "`4Disable";
            }
            if (wrenchkick == true) {
                totkick = "`2Enable";
            } else {
                totkick = "`4Disable";
            }
            if (wrenchban == true) {
                totban = "`2Enable";
            } else {
                totban = "`4Disable";
            }
            if (wrenchtrade == true) {
                tottrade = "`2Enable";
            } else {
                tottrade = "`4Disable";
            }
            if (wrenchadd == true) {
                totadd = "`2Enable";
            } else {
                totadd = "`4Disable";
            }
            if (wrenchtp == true) {
                tottp = "`2Enable";
            } else {
                tottp = "`4Disable";
            }
            Dialog a;
            a.addLabelWithIcon("Wrench Option", 32, LABEL_BIG);
            a.addSpacer(SPACER_SMALL);
            a.addTextBox("Wrench Mode :");
            a.addTextBox("[1] Wrench Pull : " + totpull);
            a.addTextBox("[2] Wrench Kick : " + totkick);
            a.addTextBox("[3] Wrench Ban : " + totban);
            a.addTextBox("[4] Wrench Trade : " + tottrade);
            a.addTextBox("[5] Wrench Add : " + totadd);
            a.addTextBox("[6] Wrench Teleport : " + tottp);
            a.addSpacer(SPACER_SMALL);
            a.addInputBox("wrenchmode_change", "Wrench Mode : ", std::to_string(wrenchmode), 3);
            a.addSpacer(SPACER_SMALL);
            a.endDialog("wrenchchch", "Done", "Cancel");
            variantlist_t liste{ "OnDialogRequest" };
            liste[1] = a.finishDialog();
            g_server->send(true, liste);
            return true;
        } else if (find_command(chat, "automsg")) {
            if (automsg == true) {
                tutitut = "1";
            }
            else {
                tutitut = "0";
            }
            std::string sosis;
            sosis =
            "add_label_with_icon|big|Auto Msg Settings|left|32|"
            "\nadd_spacer|small"
            "\nadd_checkbox|message_1|Auto Message|" + tutitut + "|"
            "\nadd_spacer|small"
            "\nadd_text_input|message_2|Message : ||30|"
            "\nend_dialog|autotomsgg_kasaak|Cancel|Okay|";
            variantlist_t ajsga{ "OnDialogRequest" };
            ajsga[1] = sosis;
            g_server->send(true, ajsga);
            return true;
        }
        return false;
    }
    if (packet.find("buttonClicked|killall") != -1)
    {
        worldoptions("kick");
    }
    if (packet.find("buttonClicked|pullall") != -1)
    {
        worldoptions("pull");

    }
    if (packet.find("buttonClicked|banall") != -1)
    {
        worldoptions("ban");

    }
    if (packet.find("buttonClicked|ubaworld") != -1)
    {
        g_server->send(false, "action|input\n|text|/uba");

    }







    if (packet.find("game_version|") != -1) {
        rtvar var = rtvar::parse(packet);
        auto mac = utils::generate_mac();
        auto hash_str = mac + "RT";
        auto hash2 = utils::hash((uint8_t*)hash_str.c_str(), hash_str.length());
        var.set("mac", mac);
        var.set("wk", utils::generate_rid());
        var.set("rid", utils::generate_rid());
        var.set("fz", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("zf", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("hash", std::to_string(utils::random(INT_MIN, INT_MAX)));
        var.set("hash2", std::to_string(hash2));
        var.set("meta", utils::random(utils::random(6, 10)) + ".com");
        var.set("game_version", gt::version);
        var.set("country", gt::flag);



        /*
        AAP Bypass
        Only making this public because after 1 month being reported to ubi, nothing happened
        Then after a month (around 15.3) it got fixed for a whole single 1 day, and they publicly said it had been fixed
        And at that time we shared how to do it because thought its useless, and then aap bypass started working again
        and then 9999 new aap bypass services came to be public, and even playingo started selling it so no point keeping it private
        With publishing this I hope ubi actually does something this time
        */

        //Finally patched, I guess they finally managed to fix this after maybe a year!

        /*if (var.find("tankIDName")) {
            var.find("mac")->m_values[0] = "02:00:00:00:00:00";
            var.find("platformID")->m_values[0] = "4"; //android
            var.remove("fz");
            var.remove("rid");
        }*/

        packet = var.serialize();
        gt::in_game = false;
        PRINTS("Spoofing login info\n");
        g_server->send(false, packet);
        return true;
    }

    return false;
}

bool events::out::gamemessage(std::string packet) {
    PRINTS("Game message: %s\n", packet.c_str());
    if (packet == "action|quit") {
        g_server->quit();
        return true;
    }

    return false;
}

// /world
bool events::out::worldoptions(std::string option)
{
    std::string username = "all";
    for (auto& player : g_server->m_world.players) {
        auto name_2 = player.name.substr(2);
        std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
        if (name_2.find(username)) {
            auto& bruh = g_server->m_world.local;
            if (option == "pull")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/pull " + plyr);
                }
            }
            if (option == "kick")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/kick " + plyr);

                }
            }
            if (option == "ban")
            {
                string plyr = player.name.substr(2).substr(0, player.name.length() - 4);
                if (plyr != bruh.name.substr(2).substr(0, player.name.length() - 4))
                {
                    g_server->send(false, "action|input\n|text|/ban " + plyr);

                }
            }
        }
    }

    return true;
}










bool events::out::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;

    g_server->m_world.local.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
    PRINTS("local pos: %.0f %.0f\n", packet->m_vec_x, packet->m_vec_y);

    if (fakeblink) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        variantlist_t varlist{"OnSetPos"};
        varlist[1] = g_server->m_world.local.pos;
        g_server->m_world.local.pos = g_server->m_world.local.pos;
        g_server->send(true, varlist, g_server->m_world.local.netid, -1);
        return true;
    }
    if (gt::ghost)
        return true;
    return false;
}

bool events::in::variantlist(gameupdatepacket_t* packet) {
    variantlist_t varlist{};
    auto extended = utils::get_extended(packet);
    extended += 4; //since it casts to data size not data but too lazy to fix this
    varlist.serialize_from_mem(extended);
    PRINTC("varlist: %s\n", varlist.print().c_str());
    auto func = varlist[0].get_string();

    //probably subject to change, so not including in switch statement.
    if (func.find("OnSuperMainStartAcceptLogon") != -1)
        gt::in_game = true;



    

    switch (hs::hash32(func.c_str())) {
        //solve captcha
        case fnv32("onShowCaptcha"): {
            auto menu = varlist[1].get_string();
            gt::solve_captcha(menu);
            return true;
        } break;
        case fnv32("OnRequestWorldSelectMenu"): {
            auto& world = g_server->m_world;
            world.players.clear();
            world.local = {};
            world.connected = false;
            world.name = "EXIT";
        } break;
        case fnv32("OnSendToServer"): g_server->redirect_server(varlist); return true;

        case fnv32("OnConsoleMessage"): {
            varlist[1] = varlist[1].get_string();
            auto cnsl = varlist[1].get_string();
            if (vendtype == true) {
                g_server->send(true, varlist);
                if (cnsl.find("changed the p") != -1) {
                    std::string gridid = cnsl.substr(cnsl.find("`7[``") + 5, cnsl.length() - cnsl.find("`7[``") - 1);
                    std::string itemnamememe = cnsl.substr(cnsl.find("of `2") + 5, cnsl.length() - cnsl.find("of `2") - 1);
                    gridid.erase(gridid.begin() + gridid.find(" changed"), gridid.end());
                    itemnamememe.erase(itemnamememe.begin() + itemnamememe.find("`` to `"), itemnamememe.end());
                    if (cnsl.find("World Locks each.") != -1) {
                        g_server->send(false, "action|input\n|text|`7[`0" + gridid + " cha`0nged the price of `2" + itemnamememe + "`0 to `61 per World Lock.`7]");
                        return true;
                    }
                }
                return false;
            }







            auto wry = varlist[1].get_string();
            if (worldbanjoinmod == true)
            {
                if (wry.find("`#[SahyuiMOD] `w: Removed your access from all locks.") != -1)
                {
                    gt::send_log("`#[SahyuiMOD] `w: `$Leaving the world due to having Mod bypass on and due to having a `#mod `$in the world!");
                    g_server->send(false, "action|join_request\nname|exit", 3);
                }
            }










            if (banscam == true) {
                g_server->send(true, varlist);
                if (cnsl.find("Skem") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("scam") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("dont") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("Scam") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("SCam") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("noob") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("lol") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                } else if (cnsl.find("LOL") != -1) {
                    if (cnsl.find("`w") != -1) {
                        std::string nasmasma = cnsl.substr(cnsl.find("[W]_ `6<`w") + 10, cnsl.length() - cnsl.find("[W]_ `6<`w") - 1);
                        nasmasma.erase(nasmasma.begin() + nasmasma.find("``>``"), nasmasma.end());
                        g_server->send(false, "action|input\n|text|/ban " + nasmasma);
                    }
                }
                return true;
            }
        } break;
        case fnv32("OnDialogRequest"): {
            auto content = varlist[1].get_string();

            if (content.find("set_default_color|`o") != -1) 
            {
                if (content.find("end_dialog|captcha_submit||Submit|") != -1) 
                {
                    gt::solve_captcha(content);
                    return true;
                }
            }
            else if (content.find("add_label_with_icon|small|Remove Your Access From World|left|242|") != -1) {
                if (worldbanjoinmod == true)
                {
                    g_server->send(false, "action|dialog_return\ndialog_name|unaccess");
                    return true;
                }
            }

        if (fastvend == true) {
            if (content.find("How many would you like to buy?") != -1) {
                int x = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int y = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                int itemid = std::stoi(content.substr(content.find("embed_data|expectitem|") + 22, content.length() - content.find("embed_data|expectitem|") - 1));
                int expectprice = std::stoi(content.substr(content.find("embed_data|expectprice|") + 23, content.length() - content.find("embed_data|expectprice|") - 1));
                int yourwl = std::stoi(content.substr(content.find("add_textbox|You have") + 20, content.length() - content.find("add_textbox|You have") - 1));
                if (yourwl - expectprice < 0) {
                } else {
                    g_server->send(false, "action|dialog_return\ndialog_name|vending\ntilex|" + std::to_string(x) + "|\ntiley|" + std::to_string(y) + "|\nverify|1|\nbuycount|" + std::to_string(amountvend) + "|\nexpectprice|" + std::to_string(expectprice) + "|\nexpectitem|" + std::to_string(itemid) + "|");
                    return true;
                }
            }
        }


        if (worldbanjoinmod == true)
        {
            //problem ola biler!
            gt::send_log("The `2Exit world when mod joins `2ON");

        }
        else
        {
            //problem!
            gt::send_log("The `2Exit world when mod joins `4OFF");

        }







        if (passforce == true) {
            if (content.find("The door requires a password") != -1) {
                int passdx = std::stoi(content.substr(content.find("embed_data|tilex|") + 17, content.length() - content.find("embed_data|tilex|") - 1));
                int passdy = std::stoi(content.substr(content.find("embed_data|tiley|") + 17, content.length() - content.find("embed_data|tiley|") - 1));
                g_server->send(false, "action|dialog_return\ndialog_name|password_reply\ntilex|" + std::to_string(passdx) + "|\ntiley|" + std::to_string(passdy) + "|\npassword|" + std::to_string(pwd));
                gt::send_log("Guess : `c" + std::to_string(pwd));
                pwd = pwd + 1;
                return true;
            }
        }



        if (fastdrop == true) {
            int itemid = std::stoi(content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1));
            int count = std::stoi(content.substr(content.find("count||") + 7, content.length() - content.find("count||") - 1));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Drop") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|drop_item\nitemID|" + std::to_string(itemid) + "|\ncount|" + std::to_string(count));
                    return true;
                }
            }
        }


        if (content.find("Please enter your password") != -1) {
            content = content.insert(content.find("add_button|show_recoveryanswer|`wRecover Password``|noflags|0|0|"),"add_button|show_recoveryanswer|`wRecover Password``|noflags|0|0|\nadd_button|bypass_safevault|`wBypass Safe Vault``|noflags|0|0|");
            varlist[1] = content;
            g_server->send(true, varlist, -1, -1);
                /*std::string saveve;
                saveve =
                    "set_default_color|`o"
                    "\nadd_label_with_icon|big|`wSafe Vault``|left|8878|"
                    "\nembed_data|tilex|" + std::to_string(vaultx) + 
                    "\nembed_data|tiley|" + std::to_string(vaulty) + 
                    "\nadd_smalltext|The ingenious minds at GrowTech bring you the `2Safe Vault`` - a place to store your items safely with its integrated password option!|left|"
                    "\nadd_smalltext|How the password works:|left|"
                    "\nadd_smalltext|The Safe Vault requires both a `2password`` and a `2recovery answer`` to be entered to use a password.|left|"
                    "\nadd_smalltext|Enter your `2password`` and `2recovery answer`` below - keep them safe and `4DO NOT`` share them with anyone you do not trust!|left|"
                    "\nadd_smalltext|The password and recovery answer can be no longer than 12 characters in length - number and alphabet only please, no special characters are allowed!|left|"
                    "\nadd_smalltext|If you forget your password, enter your recovery answer to access the Safe Vault - The Safe Vault will `4NOT be password protected now``. You will need to enter a new password.|left|"
                    "\nadd_smalltext|You can change your password, however you will need to enter the old password before a new one can be used.|left|"
                    "\nadd_smalltext|`4WARNING``: DO NOT forget your password and recovery answer or you will not be able to access the Safe Vault!|left|"
                    "\nadd_textbox|`4There is no password currently set on this Safe Vault.``|left|"
                    "\nadd_textbox|Enter a new password.|left|"
                    "\nadd_text_input_password|storage_newpassword|||18|"
                    "\nadd_textbox|Enter a recovery answer.|left|"
                    "\nadd_text_input|storage_recoveryanswer|||12|"
                    "\nadd_button|set_password|Update Password|noflags|0|0|"
                    "\nend_dialog|storageboxpassword|Exit||"
                    "\nadd_quick_exit|";
                variantlist_t lsals{ "OnDialogRequest" };
                lsals[1] = saveve;
                g_server->send(true, lsals);*/
            return true;
        }



        if (wrenchmodeon == true) {
            std::string tototot = content.substr(content.find("embed_data|netID") + 17, content.length() - content.find("embed_data|netID") - 1);
            if (content.find("set_default_color|`o") != -1) {
                if (content.find("embed_data|netID") != -1) {
                    return true;
                }
            }
        } else if (wrenchtp == true) {
            if (wrenchmodeon == false) {
                if (content.find("set_default_color|`o") != -1) {
                    if (content.find("embed_data|netID") != -1) {
                        if (content.find("add_label_with_icon|big|`w") != -1) {
                            std::string alsja =
                                content.substr(content.find("add_label_with_icon|big|`w") + 26, content.length() - content.find("add_label_with_icon|big|`w") - 1);
                            alsja.erase(alsja.begin() + alsja.find(" (`2"), alsja.end());
                            std::string name = alsja;
                            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                            for (auto& player : g_server->m_world.players) {
                                auto name_2 = player.name.substr(2); //remove color
                                std::transform(name_2.begin(), name_2.end(), name_2.begin(), ::tolower);
                                if (name_2.find(name) == 0) {
                                    gt::send_log("Teleporting To " + player.name);
                                    variantlist_t varlist{ "OnSetPos" };
                                    varlist[1] = player.pos;
                                    g_server->m_world.local.pos = player.pos;
                                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                                    break;
                                }
                            }
                        }
                        return true;
                    }
                }
            }
        }






        if (fasttrash == true) {
            std::string itemid = content.substr(content.find("embed_data|itemID|") + 18, content.length() - content.find("embed_data|itemID|") - 1);
            std::string count = content.substr(content.find("you have ") + 9, content.length() - content.find("you have ") - 1);
            std::string konz = count.substr(0,count.find(")"));
            if (content.find("embed_data|itemID|") != -1) {
                if (content.find("Trash") != -1) {
                    g_server->send(false, "action|dialog_return\ndialog_name|trash_item\nitemID|" + itemid + "|\ncount|" + konz);
                    return true;
                }
            }
        }

            if (gt::resolving_uid2 && (content.find("friend_all|Show offline") != -1 || content.find("Social Portal") != -1) ||
                content.find("Are you sure you wish to stop ignoring") != -1) {
                return true;
            } else if (gt::resolving_uid2 && content.find("Ok, you will now be able to see chat") != -1) {
                gt::resolving_uid2 = false;
                return true;
            } else if (gt::resolving_uid2 && content.find("`4Stop ignoring") != -1) {
                int pos = content.rfind("|`4Stop ignoring");
                auto ignore_substring = content.substr(0, pos);
                auto uid = ignore_substring.substr(ignore_substring.rfind("add_button|") + 11);
                auto uid_int = atoi(uid.c_str());
                if (uid_int == 0) {
                    gt::resolving_uid2 = false;
                    gt::send_log("`9UID Resolve `4Failed");
                } else {
                    gt::send_log("`9Target UID: `c" + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|" + uid);
                    g_server->send(false, "action|dialog_return\ndialog_name|friends_remove\nfriendID|" + uid + "|\nbuttonClicked|remove");
                }
                return true;
            } 
        } break;

        case fnv32("OnSetPos"): {
            if (gt::noclip)
                return true;
            return false;
        } break;








        case fnv32("OnRemove"): {
            auto text = varlist.get(1).get_string();
            if (text.find("netID|") == 0) {
                auto netid = atoi(text.substr(6).c_str());

                if (netid == g_server->m_world.local.netid)
                    g_server->m_world.local = {};

                auto& players = g_server->m_world.players;
                for (size_t i = 0; i < players.size(); i++) {
                    auto& player = players[i];
                    if (player.netid == netid) {
                        players.erase(std::remove(players.begin(), players.end(), player), players.end());
                        break;
                    }
                }
            }
        } break;
        case fnv32("OnTalkBubble"): {
            auto bbl = varlist[2].get_string();
            if (passforce == true) {
                if (bbl.find("`2The door opens!") != -1) {
                    pwd = 0;
                    return true;
                } 
            } else {
                g_server->send(true, varlist);
                return true;
            }




        } break;
        case fnv32("OnTextOverlay"): {
            std::string overlay = varlist.get(1).get_string();
            if (autorespull == true) {
                if (overlay.find("You were pulled by") != -1) {
                    g_server->send(true, varlist);
                    g_server->send(false, "action|respawn");
                }
                else {
                    g_server->send(true, varlist);
                }
                return true;
            }
            return true;
        } break;
        case fnv32("OnSpawn"): {
            std::string meme = varlist.get(1).get_string();
            rtvar var = rtvar::parse(meme);
            auto name = var.find("name");
            auto netid = var.find("netID");
            auto onlineid = var.find("onlineID");

           
            

            
            if (name && netid && onlineid) {
                player ply{};

                



                if (var.find("invis")->m_value != "1") {
                    ply.name = name->m_value;
                    ply.country = var.get("country");
                    //name->m_values[0] += " `4[" + netid->m_value + "]``";
                    auto pos = var.find("posXY");
                    if (pos && pos->m_values.size() >= 2) {
                        auto x = atoi(pos->m_values[0].c_str());
                        auto y = atoi(pos->m_values[1].c_str());
                        ply.pos = vector2_t{ float(x), float(y) };
                    }
                } else {
                    ply.mod = true;
                    ply.invis = true;
                }
                if (var.get("mstate") == "1" || var.get("smstate") == "1")
                    ply.mod = true;
                ply.userid = var.get_int("userID");
                ply.netid = var.get_int("netID");
                if (meme.find("type|local") != -1) {
                    //set mod state to 1 (allows infinite zooming, this doesnt ban cuz its only the zoom not the actual long punch)
                    var.find("mstate")->m_values[0] = "1";
                    g_server->m_world.local = ply;
                }
                
                g_server->m_world.players.push_back(ply);
                auto str = var.serialize();
                utils::replace(str, "onlineID", "onlineID|");
                varlist[1] = str;
                PRINTC("new: %s\n", varlist.print().c_str());
                g_server->send(true, varlist, -1, -1);
                if (automsg == true) {
                    try {
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        g_server->send(false, "action|input\n|text|/msg " + ply.name + messagelolos);
                    } catch (std::exception) { gt::send_log("Critical Error : Invalid String Position"); }
                }
                return true;
            }

            
        } break;
    }
    return false;
}

bool events::in::generictext(std::string packet) {
    PRINTC("Generic text: %s\n", packet.c_str());




    return false;
}



bool events::in::gamemessage(std::string packet) {
    PRINTC("Game message: %s\n", packet.c_str());

    if (gt::resolving_uid2) {
        if (packet.find("PERSON IGNORED") != -1) {
            g_server->send(false, "action|dialog_return\ndialog_name|friends_guilds\nbuttonClicked|showfriend");
            g_server->send(false, "action|dialog_return\ndialog_name|friends\nbuttonClicked|friend_all");
        } else if (packet.find("Nobody is currently online with the name") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("`9Target Is Offline, Cant Find UID");
        } else if (packet.find("Clever perhaps") != -1) {
            gt::resolving_uid2 = false;
            gt::send_log("`9Target Is Moderator, Can't Ignore Them.");
        }
    }
    return false;
}











bool events::in::sendmapdata(gameupdatepacket_t* packet) {
    g_server->m_world = {};
    auto extended = utils::get_extended(packet);
    extended += 4;
    auto data = extended + 6;
    auto name_length = *(short*)data;

    char* name = new char[name_length + 1];
    memcpy(name, data + sizeof(short), name_length);
    char none = '\0';
    memcpy(name + name_length, &none, 1);

    g_server->m_world.name = std::string(name);
    g_server->m_world.connected = true;
    delete[] name;
    PRINTC("world name is %s\n", g_server->m_world.name.c_str());
    return false;
}

bool events::in::state(gameupdatepacket_t* packet) {
    if (!g_server->m_world.connected)
        return false;
    if (packet->m_player_flags == -1)
        return false;

    auto& players = g_server->m_world.players;
    for (auto& player : players) {
        if (player.netid == packet->m_player_flags) {
            player.pos = vector2_t{ packet->m_vec_x, packet->m_vec_y };
            PRINTC("player %s position is %.0f %.0f\n", player.name.c_str(), player.pos.m_x, player.pos.m_y);
            /*if (followply == true) {
                if (player.name.c_str() == namesusi) {
                    variantlist_t varlist{ "OnSetPos" };
                    varlist[1] = player.pos;
                    g_server->m_world.local.pos = player.pos;
                    g_server->send(true, varlist, g_server->m_world.local.netid, -1);
                }
            }*/
            break;
        }
    }
    return false;
}

bool events::in::tracking(std::string packet) {
    PRINTC("Tracking packet: %s\n", packet.c_str());
    return true;
}
