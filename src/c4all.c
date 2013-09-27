/**
 * @file c4all.c
 * @brief Handle compo 4 all stuffs
 * @created 2013-09-26
 * @date 2013-09-26
 */
/*
 * Copyright (C) 2013 Pierre Jost
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "powermanga.h"
#include "log_recorder.h"
#include "c4all.h"

#undef PREFIX
#include <sparrowNet.h>

char defaultName[4] = "\0\0\0\0";

char* GetDefaultName(){
    if(defaultName[0]=='\0'){
        spInitNet();
        spNetC4AProfilePointer profile = spNetC4AGetProfile();
        if(profile!=NULL){
            for(int i = 0;i<3;i++){
                defaultName[i]=profile->shortname[i];
            }
        }else{
            defaultName[0] = ' ';
            defaultName[1] = ' ';
            defaultName[2] = ' ';
        }
        spQuitNet();
    }
    return defaultName;
}

/**
 * Submit a new score to c4all server
 * @param difficulty game difficulty
 * @param score the score to publish
 * @return return true when successfull
 */
bool PublishScore(int difficulty, long score){
    char* c4allGameName;
    switch(difficulty){
    case 0:
        c4allGameName = "PowerManga_e";
        break;
    case 1:
        c4allGameName = "PowerManga_n";
        break;
    case 2:
        c4allGameName = "PowerManga_h";
        break;
    default:
        LOG_ERR("<c4all - PublishScore> unknown difficulty");
        return false;
    }
    LOG_INF("<c4all> - %s - Score: %lu",c4allGameName,score);

    spInitNet();
    spNetC4AProfilePointer profile = spNetC4AGetProfile();
    if (profile!=NULL){
        spNetC4AScorePointer pointsScore = NULL;
        spNetC4ACommitScore(profile,c4allGameName,score,&pointsScore,5000); //Timeout 5 Seconds
        while (spNetC4AGetStatus() == SP_C4A_PROGRESS){ //Wait for the thread
            SDL_Delay(200);
        }
        switch(spNetC4AGetStatus()){
            case SP_C4A_CANCELED:
                LOG_ERR("Online score uploading canceled!\n");
            break;
            case SP_C4A_TIMEOUT:
                LOG_ERR("Online score uploading resulted in a timeout\n");
            break;
            case SP_C4A_PROGRESS:
                LOG_ERR("Online score uploading did not finish.\n");
            break;
            case SP_C4A_OK:
                LOG_INF("Online score uploading went fine.\n");
            break;
            case SP_C4A_ERROR:
                LOG_ERR("Online score uploading reported an error.\n");
            break;
        }
    }else{
        LOG_ERR("No C4A profile found.\n");
    }
    spQuitNet();
}
