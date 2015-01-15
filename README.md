Le-jeu-4

Pour rappel, 5 minutes de présentation par personne (soir 5 pour un monôme, 10 pour un binôme et 15 pour un trinôme — comme quoi ça aide de réviser ses tables de multiplication !) qui seront suivies de 5 minutes de questions (soit 5 minutes pour un monôme, 5 minutes pour un binôme et 5 minutes pour un trinôme).


Choses à faire par ordre de priorité :

-Virer toutes les variables globales du programme
-Commentaires et descriptions des fonctions
-Mode de jeu Classique : Deux camps, pas d'IA, chaque joueur dirige une équipe variée.
-Mode de jeu Survie : Autant de camps que nécessaire, un perso par joueur, des monstres puissants mais peu résistants apparaissent aléatoirement sur la carte et frappent aléatoirement autour d'eux, y compris leurs alliés.
-Mode de jeu Mort Subite : Deux camps, pas d'IA, comme un jeu d'échec.

Il faut remplacer les nombres magiques 65 66 et autres correspondant à un appui sur une touche directionnelle par des constantes nommées plus explicitement.

Annuler n'annule pas l'action. Cela nous renvoie au début de l'action, mais ne l'annule pas.





========
A voir en Raw.

http://patorjk.com/software/taag/#p=testall&c=c&f=Invita&t=Le%20jeu%204


                                                                                                                                   
                                                                                                                                   \n
 ███████████                                                  ████                                                   █████████\n
 █         █                                                 █    █                                                 █        █\n
 █         █                                                  ████                                                 █         █\n
 ██       ██                                                                                                      █    ██    █\n
   █     █                   ████████████                   ███████    ████████████    ██████    ██████          █    █ █    █\n
   █     █                 ██            ██                 █     █  ██            ██  █    █    █    █         █    █  █    █\n
   █     █                █      █████     ██                █    █ █      █████     ███    █    █    █        █    █   █    █\n
   █     █               █      █     █     █                █    ██      █     █     ██    █    █    █       █    ██████    ███\n
   █     █               █       █████      █                █    ██       █████      ██    █    █    █       █                █\n
   █     █               █                 █                 █    ██                 █ █    █    █    █       ██████████     ███\n
   █     █               █      ███████████                  █    ██      ███████████  █    █    █    █                 █    █\n
   █     █         ███████       █                           █    ██       █           █     ████     █                 █    █\n
 ██       █████████     ██        █                          █    ██        █          █               ██               █    █\n
 █                      █ █        ████████                  █    █ █        ████████   █               █             ██      ██\n
 █                      █  ██             █                  █    █  ██             █    ██        ██   █             █        █\n
 ████████████████████████    ██████████████                  █    █    ██████████████      ████████  ████             ██████████\n
                                                             █    █\n
                                                   ████      █    █\n
                                                  █    ██   █     █\n
                                                  █      ███      █\n
                                                   ██            █\n
                                                     ███      ███\n
                                                        ██████\n
\n
                   _                                 _      _                  _ _ _   _\n
    ___ _   _  ___| |__     ___ ___  _ __ ___  _ __ | | ___| |_ ___    ___  __| (_) |_(_) ___  _ __   __      _______      __\n
   / __| | | |/ __| '_ \   / __/ _ \| '_ ` _ \| '_ \| |/ _ \ __/ _ \  / _ \/ _` | | __| |/ _ \| '_ \  \ \ /\ / / _ \ \ /\ / /\n
---\__ \ |_| | (__| | | | | (_| (_) | | | | | | |_) | |  __/ ||  __/ |  __/ (_| | | |_| | (_) | | | |  \ V  V / (_) \ V  V / ---\n
   |___/\__,_|\___|_| |_|  \___\___/|_| |_| |_| .__/|_|\___|\__\___|  \___|\__,_|_|\__|_|\___/|_| |_|   \_/\_/ \___/ \_/\_/\n
                                             |_|                                                                            


























/***
 *    ██╗     ███████╗         ██╗███████╗██╗   ██╗    ██╗  ██╗
 *    ██║     ██╔════╝         ██║██╔════╝██║   ██║    ██║  ██║
 *    ██║     █████╗           ██║█████╗  ██║   ██║    ███████║
 *    ██║     ██╔══╝      ██   ██║██╔══╝  ██║   ██║    ╚════██║
 *    ███████╗███████╗    ╚█████╔╝███████╗╚██████╔╝         ██║
 *    ╚══════╝╚══════╝     ╚════╝ ╚══════╝ ╚═════╝          ╚═╝
 *                                                             
 */

ANSI Shadow

/***
 *         _           _____          _     
 *     ___/__)        (, /            /   / 
 *    (, /    _         /   _        /___/_ 
 *      /   _(/_    ___/___(/_(_(_      /   
 *     (_____     /   /                /    
 *            )  (__ /                      
 */

Invita

/***
 *      _               _              _  _   
 *     | |    ___      | | ___ _   _  | || |  
 *     | |   / _ \  _  | |/ _ \ | | | | || |_ 
 *     | |__|  __/ | |_| |  __/ |_| | |__   _|
 *     |_____\___|  \___/ \___|\__,_|    |_|  
 *                                            
 */

Ivrit

/***
 *           ,gggg,                    gg                                      a8   
 *          d8" "8I                   dP8,                                   ,d88   
 *          88  ,dP                  dP Yb                                  a8P88   
 *       8888888P"                  ,8  `8,                               ,d8" 88   
 *          88                      I8   Yb                              a8P'  88   
 *          88         ,ggg,        `8b, `8,    ,ggg,   gg      gg     ,d8"    88   
 *     ,aa,_88        i8" "8i        `"Y88888  i8" "8i  I8      8I     888888888888 
 *    dP" "88P        I8, ,8I            "Y8   I8, ,8I  I8,    ,8I             88   
 *    Yb,_,d88b,,_    `YbadP'             ,88, `YbadP' ,d8b,  ,d8b,            88   
 *     "Y8P"  "Y88888888P"Y888        ,ad88888888P"Y8888P'"Y88P"`Y8            88   
 *                                  ,dP"'   Yb                                      
 *                                 ,8'      I8                                      
 *                                ,8'       I8                                      
 *                                I8,      ,8'                                      
 *                                `Y8,___,d8'                                       
 *                                  "Y888P"                                         
 */
/***
 *                     _                                 _      _                  _ _ _   _                                     
 *      ___ _   _  ___| |__     ___ ___  _ __ ___  _ __ | | ___| |_ ___    ___  __| (_) |_(_) ___  _ __   __      _______      __
 *     / __| | | |/ __| '_ \   / __/ _ \| '_ ` _ \| '_ \| |/ _ \ __/ _ \  / _ \/ _` | | __| |/ _ \| '_ \  \ \ /\ / / _ \ \ /\ / /
 *     \__ \ |_| | (__| | | | | (_| (_) | | | | | | |_) | |  __/ ||  __/ |  __/ (_| | | |_| | (_) | | | |  \ V  V / (_) \ V  V / 
 *     |___/\__,_|\___|_| |_|  \___\___/|_| |_| |_| .__/|_|\___|\__\___|  \___|\__,_|_|\__|_|\___/|_| |_|   \_/\_/ \___/ \_/\_/  
 *                                                |_|                                                                            
 */

/***
 *                                                                                                                                                                                                                                                                     
 *                                     ,dPYb,                                                               ,dPYb,            I8                                  8I          I8                                                                                       
 *                                     IP'`Yb                                                               IP'`Yb            I8                                  8I          I8                                                                                       
 *                                     I8  8I                                                               I8  8I         88888888                               8I   gg  88888888  gg                                                                                
 *                                     I8  8'                                                               I8  8'            I8                                  8I   ""     I8     ""                                                                                
 *       ,g,     gg      gg    ,gggg,  I8 dPgg,         ,gggg,    ,ggggg,     ,ggg,,ggg,,ggg,   gg,gggg,    I8 dP   ,ggg,     I8     ,ggg,        ,ggg,     ,gggg,8I   gg     I8     gg     ,ggggg,     ,ggg,,ggg,       gg    gg    gg     ,ggggg,    gg    gg    gg  
 *      ,8'8,    I8      8I   dP"  "Yb I8dP" "8I       dP"  "Yb  dP"  "Y8ggg ,8" "8P" "8P" "8,  I8P"  "Yb   I8dP   i8" "8i    I8    i8" "8i      i8" "8i   dP"  "Y8I   88     I8     88    dP"  "Y8ggg ,8" "8P" "8,      I8    I8    88bg  dP"  "Y8ggg I8    I8    88bg
 *     ,8'  Yb   I8,    ,8I  i8'       I8P    I8      i8'       i8'    ,8I   I8   8I   8I   8I  I8'    ,8i  I8P    I8, ,8I   ,I8,   I8, ,8I      I8, ,8I  i8'    ,8I   88    ,I8,    88   i8'    ,8I   I8   8I   8I      I8    I8    8I   i8'    ,8I   I8    I8    8I  
 *    ,8'_   8) ,d8b,  ,d8b,,d8,_    _,d8     I8,    ,d8,_    _,d8,   ,d8'  ,dP   8I   8I   Yb,,I8 _  ,d8' ,d8b,_  `YbadP'  ,d88b,  `YbadP'      `YbadP' ,d8,   ,d8b,_,88,_ ,d88b, _,88,_,d8,   ,d8'  ,dP   8I   Yb,    ,d8,  ,d8,  ,8I  ,d8,   ,d8'  ,d8,  ,d8,  ,8I  
 *    P' "YY8P8P8P'"Y88P"`Y8P""Y8888PP88P     `Y8    P""Y8888PPP"Y8888P"    8P'   8I   8I   `Y8PI8 YY88888P8P'"Y88888P"Y88888P""Y88888P"Y888    888P"Y888P"Y8888P"`Y88P""Y888P""Y888P""Y8P"Y8888P"    8P'   8I   `Y8    P""Y88P""Y88P"   P"Y8888P"    P""Y88P""Y88P"   
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 */

Nscript

/***
 *                                                                                                                                   
 *                                                                                                                                   
 *    LLLLLLLLLLL                                                  jjjj                                                   444444444  
 *    L:::::::::L                                                 j::::j                                                 4::::::::4  
 *    L:::::::::L                                                  jjjj                                                 4:::::::::4  
 *    LL:::::::LL                                                                                                      4::::44::::4  
 *      L:::::L                   eeeeeeeeeeee                   jjjjjjj    eeeeeeeeeeee    uuuuuu    uuuuuu          4::::4 4::::4  
 *      L:::::L                 ee::::::::::::ee                 j:::::j  ee::::::::::::ee  u::::u    u::::u         4::::4  4::::4  
 *      L:::::L                e::::::eeeee:::::ee                j::::j e::::::eeeee:::::eeu::::u    u::::u        4::::4   4::::4  
 *      L:::::L               e::::::e     e:::::e                j::::je::::::e     e:::::eu::::u    u::::u       4::::444444::::444
 *      L:::::L               e:::::::eeeee::::::e                j::::je:::::::eeeee::::::eu::::u    u::::u       4::::::::::::::::4
 *      L:::::L               e:::::::::::::::::e                 j::::je:::::::::::::::::e u::::u    u::::u       4444444444:::::444
 *      L:::::L               e::::::eeeeeeeeeee                  j::::je::::::eeeeeeeeeee  u::::u    u::::u                 4::::4  
 *      L:::::L         LLLLLLe:::::::e                           j::::je:::::::e           u:::::uuuu:::::u                 4::::4  
 *    LL:::::::LLLLLLLLL:::::Le::::::::e                          j::::je::::::::e          u:::::::::::::::uu               4::::4  
 *    L::::::::::::::::::::::L e::::::::eeeeeeee                  j::::j e::::::::eeeeeeee   u:::::::::::::::u             44::::::44
 *    L::::::::::::::::::::::L  ee:::::::::::::e                  j::::j  ee:::::::::::::e    uu::::::::uu:::u             4::::::::4
 *    LLLLLLLLLLLLLLLLLLLLLLLL    eeeeeeeeeeeeee                  j::::j    eeeeeeeeeeeeee      uuuuuuuu  uuuu             4444444444
 *                                                                j::::j                                                             
 *                                                      jjjj      j::::j                                                             
 *                                                     j::::jj   j:::::j                                                             
 *                                                     j::::::jjj::::::j                                                             
 *                                                      jj::::::::::::j                                                              
 *                                                        jjj::::::jjj                                                               
 *                                                           jjjjjj                                                                  
 */

/***
 *                                                                                                                                                                                                                                                                     
 *                                     ,dPYb,                                                               ,dPYb,            I8                                  8I          I8                                                                                       
 *                                     IP'`Yb                                                               IP'`Yb            I8                                  8I          I8                                                                                       
 *                                     I8  8I                                                               I8  8I         88888888                               8I   gg  88888888  gg                                                                                
 *                                     I8  8'                                                               I8  8'            I8                                  8I   ""     I8     ""                                                                                
 *       ,g,     gg      gg    ,gggg,  I8 dPgg,         ,gggg,    ,ggggg,     ,ggg,,ggg,,ggg,   gg,gggg,    I8 dP   ,ggg,     I8     ,ggg,        ,ggg,     ,gggg,8I   gg     I8     gg     ,ggggg,     ,ggg,,ggg,       gg    gg    gg     ,ggggg,    gg    gg    gg  
 *      ,8'8,    I8      8I   dP"  "Yb I8dP" "8I       dP"  "Yb  dP"  "Y8ggg ,8" "8P" "8P" "8,  I8P"  "Yb   I8dP   i8" "8i    I8    i8" "8i      i8" "8i   dP"  "Y8I   88     I8     88    dP"  "Y8ggg ,8" "8P" "8,      I8    I8    88bg  dP"  "Y8ggg I8    I8    88bg
 *     ,8'  Yb   I8,    ,8I  i8'       I8P    I8      i8'       i8'    ,8I   I8   8I   8I   8I  I8'    ,8i  I8P    I8, ,8I   ,I8,   I8, ,8I      I8, ,8I  i8'    ,8I   88    ,I8,    88   i8'    ,8I   I8   8I   8I      I8    I8    8I   i8'    ,8I   I8    I8    8I  
 *    ,8'_   8) ,d8b,  ,d8b,,d8,_    _,d8     I8,    ,d8,_    _,d8,   ,d8'  ,dP   8I   8I   Yb,,I8 _  ,d8' ,d8b,_  `YbadP'  ,d88b,  `YbadP'      `YbadP' ,d8,   ,d8b,_,88,_ ,d88b, _,88,_,d8,   ,d8'  ,dP   8I   Yb,    ,d8,  ,d8,  ,8I  ,d8,   ,d8'  ,d8,  ,d8,  ,8I  
 *    P' "YY8P8P8P'"Y88P"`Y8P""Y8888PP88P     `Y8    P""Y8888PPP"Y8888P"    8P'   8I   8I   `Y8PI8 YY88888P8P'"Y88888P"Y88888P""Y88888P"Y888    888P"Y888P"Y8888P"`Y88P""Y888P""Y888P""Y8P"Y8888P"    8P'   8I   `Y8    P""Y88P""Y88P"   P"Y8888P"    P""Y88P""Y88P"   
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 *                                                                                              I8                                                                                                                                                                     
 */
Doh

/***
 *     _              _                ___ 
 *    | |            (_)              /   |
 *    | |     ___     _  ___ _   _   / /| |
 *    | |    / _ \   | |/ _ \ | | | / /_| |
 *    | |___|  __/   | |  __/ |_| | \___  |
 *    \_____/\___|   | |\___|\__,_|     |_/
 *                  _/ |                   
 *                 |__/                    
 */

Doom

/***
 *       __          _              _  _   
 *      / /  ___    (_) ___ _   _  | || |  
 *     / /  / _ \   | |/ _ \ | | | | || |_ 
 *    / /__|  __/   | |  __/ |_| | |__   _|
 *    \____/\___|  _/ |\___|\__,_|    |_|  
 *                |__/                     
 */

/***
 *                    _                                 _      _                  _ _ _   _                                     
 *     ___ _   _  ___| |__     ___ ___  _ __ ___  _ __ | | ___| |_ ___    ___  __| (_) |_(_) ___  _ __   __      _______      __
 *    / __| | | |/ __| '_ \   / __/ _ \| '_ ` _ \| '_ \| |/ _ \ __/ _ \  / _ \/ _` | | __| |/ _ \| '_ \  \ \ /\ / / _ \ \ /\ / /
 *    \__ \ |_| | (__| | | | | (_| (_) | | | | | | |_) | |  __/ ||  __/ |  __/ (_| | | |_| | (_) | | | |  \ V  V / (_) \ V  V / 
 *    |___/\__,_|\___|_| |_|  \___\___/|_| |_| |_| .__/|_|\___|\__\___|  \___|\__,_|_|\__|_|\___/|_| |_|   \_/\_/ \___/ \_/\_/  
 *                                               |_|                                                                            
 */

Ogre

/***
 *                                     
 *     __             _            ___ 
 *    |  |   ___     |_|___ _ _   | | |
 *    |  |__| -_|    | | -_| | |  |_  |
 *    |_____|___|   _| |___|___|    |_|
 *                 |___|               
 */

Rectangles

/***
 *    ooooo                           o8o                                  .o   
 *    `888'                           `"'                                .d88   
 *     888          .ooooo.          oooo  .ooooo.  oooo  oooo         .d'888   
 *     888         d88' `88b         `888 d88' `88b `888  `888       .d'  888   
 *     888         888ooo888          888 888ooo888  888   888       88ooo888oo 
 *     888       o 888    .o          888 888    .o  888   888            888   
 *    o888ooooood8 `Y8bod8P'          888 `Y8bod8P'  `V88V"V8P'          o888o  
 *                                    888                                       
 *                                .o. 88P                                       
 *                                `Y888P                                        
 */

Roman
/***
 *                                                                                                      
 *                 _                        _     _                _ _ _   _                            
 *     ___ _ _ ___| |_    ___ ___ _____ ___| |___| |_ ___    ___ _| |_| |_|_|___ ___    _ _ _ ___ _ _ _ 
 *    |_ -| | |  _|   |  |  _| . |     | . | | -_|  _| -_|  | -_| . | |  _| | . |   |  | | | | . | | | |
 *    |___|___|___|_|_|  |___|___|_|_|_|  _|_|___|_| |___|  |___|___|_|_| |_|___|_|_|  |_____|___|_____|
 *                                     |_|                                                              
 */
