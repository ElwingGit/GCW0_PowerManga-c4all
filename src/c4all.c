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

bool TestConnection();
void TryInsertHighScore(Sint32 high_scores_points[MAX_OF_HIGH_SCORES], char high_scores_names[MAX_OF_HIGH_SCORES][4], spNetC4AScorePointer score);
void TryUploadLocalScore(Sint32 high_scores_points[MAX_OF_HIGH_SCORES], char high_scores_names[MAX_OF_HIGH_SCORES][4],spNetC4AScorePointer score);

char* GetDefaultName(){
    if(defaultName[0]=='\0'){
        spInitNet();
        spNetC4AProfilePointer profile = spNetC4AGetProfile();
        if(profile!=NULL){
            for(int i = 0;i<3;i++){
                defaultName[i]=profile->shortname[i];
            }
            spNetC4AFreeProfile(profile);
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
 */
void PublishScore(int difficulty, long score){
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
        return;
    }
    LOG_INF("<c4all> - %s - Score: %lu",c4allGameName,score);

    spInitNet();

    if(!TestConnection())
        return;

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
        spNetC4AFreeProfile(profile);
    }else{
        LOG_ERR("No C4A profile found.\n");
    }
    spQuitNet();
}

/**
 * try to update highscores from c4all server
 * it will also try to upload the highest high score from the current profile
 * @param high_scores_points the table of high core value
 * @param high_scores_names the table of highscore name
 */
void TryUpdateScoreFromC4all(Sint32 high_scores_points[MAX_OF_HIGH_SCORES], char high_scores_names[MAX_OF_HIGH_SCORES][4]){
    spNetC4AScorePointer scoreList;
    spInitNet();

    if(!TestConnection())
        return;

    //request online score
    if(spNetC4AGetScore(&scoreList,NULL,"PowerManga_n",5000)==-1){
        LOG_ERR("Online score downloading failed to start!\n");
    }
    else {
        while (spNetC4AGetStatus() == SP_C4A_PROGRESS){ //Wait for the thread
            SDL_Delay(200);
        }
        switch(spNetC4AGetStatus()){
            case SP_C4A_CANCELED:
                LOG_ERR("Online score downloading canceled!\n");
            break;
            case SP_C4A_TIMEOUT:
                LOG_ERR("Online score downloading resulted in a timeout\n");
            break;
            case SP_C4A_PROGRESS:
                LOG_ERR("Online score downloading did not finish.\n");
            break;
            case SP_C4A_OK:
                LOG_INF("Online score downloading went fine.\n");
                //try to upload local scores
                TryUploadLocalScore(high_scores_points, high_scores_names, scoreList);
                //parse the list
                spNetC4AScorePointer score = scoreList;
                while(score){
                    TryInsertHighScore(high_scores_points, high_scores_names, score);
                    score = score->next;
                }

                spNetC4ADeleteScores(&scoreList);
            break;
            case SP_C4A_ERROR:
                LOG_ERR("Online score downloading reported an error.\n");
            break;
        }
    }
    spQuitNet();
}

void TryInsertHighScore(Sint32 high_scores_points[MAX_OF_HIGH_SCORES], char high_scores_names[MAX_OF_HIGH_SCORES][4], spNetC4AScorePointer score){
    int i,j;
    if(score->score > high_scores_points[MAX_OF_HIGH_SCORES-1]){
        //this is a highscore insert it
        for (i = 0; i < MAX_OF_HIGH_SCORES; i++){
            //test if the name is already present...
            if((high_scores_names[i][0] == score->shortname[0])&&(high_scores_names[i][1] == score->shortname[1])&&(high_scores_names[i][2] == score->shortname[2])){
                break;
            }
            if (score->score > high_scores_points[i]){
                for (j = MAX_OF_HIGH_SCORES - 1; j > i; j--){
                    high_scores_points[j] = high_scores_points[j - 1];
                    high_scores_names[j][0] = high_scores_names[j - 1][0];
                    high_scores_names[j][1] = high_scores_names[j - 1][1];
                    high_scores_names[j][2] = high_scores_names[j - 1][2];
                }
                high_scores_points[i] = score->score;
                high_scores_names[i][0] = score->shortname[0];
                high_scores_names[i][1] = score->shortname[1];
                high_scores_names[i][2] = score->shortname[2];
                break;
            }
        }
    }
}

void TryUploadLocalScore(Sint32 high_scores_points[MAX_OF_HIGH_SCORES], char high_scores_names[MAX_OF_HIGH_SCORES][4],spNetC4AScorePointer scoreList){
    int i;
    spNetC4AProfilePointer profile = spNetC4AGetProfile();
    if (profile==NULL){
        return;
    }

    for (i = 0; i < MAX_OF_HIGH_SCORES; i++){
        if((high_scores_names[i][0] == profile->shortname[0])&&(high_scores_names[i][1] == profile->shortname[1])&&(high_scores_names[i][2] == profile->shortname[2])){
            //we found the bestlocal user highscore, trying to submit it...
            spNetC4ACommitScore(profile,"PowerManga_n",high_scores_points[i],&scoreList,5000); //Timeout 5 Seconds
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

            break;
        }
    }

    spNetC4AFreeProfile(profile);
}

bool TestConnection(){
    spNetC4AProfilePointer profile = spNetC4AGetProfile();
    if (profile!=NULL){
        spNetC4AFreeProfile(profile);
        return true;
    }else{
        return false;
    }
    //spNetResolve is even slower than my 5second timeout for the score fetching operation...
	/*spNetIP ip = spNetResolve("skeezix.wallednetworks.com",13001);
    LOG_ERR ("adresse: %d", ip.address.ipv4);
    return ip.address.ipv4 != SP_INVALID_IP;*/
}
