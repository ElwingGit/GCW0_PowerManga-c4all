/**
 * @file c4all.h
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
#ifndef C4ALL_H_INCLUDED
#define C4ALL_H_INCLUDED
#ifdef __cplusplus
extern "C"
{
#endif
  char* GetDefaultName();
  bool PublishScore(int difficulty, long score);

#ifdef __cplusplus
}
#endif
#endif // C4ALL_H_INCLUDED
