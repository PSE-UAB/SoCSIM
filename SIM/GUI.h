/*!
 \file GUI.h
 \brief Main GUI 
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


#ifndef GUI_H_
#define GUI_H_

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * @brief Initializes GUI
 */
void gui_create();

/**
 * @brief Output character by the virtual trace
 * @param c character to out
 */
void gui_add_trace(char c);

#ifdef __cplusplus
}
#endif

#endif
