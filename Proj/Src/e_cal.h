#ifndef __TEST5_H
#define __TEST5_H

/**
 * @mainpage MinCAL
 * @section intro_sec Introduction
 * MinCAL is an application developed in the context of <a href="https://sigarra.up.pt/feup/pt/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=399888">LCOM</a>.
 * It depends heavily on device drivers developed specifically for this use.
 *
 * Authors:
 *  - Daniel Silva 		daniel.s@fe.up.pt
 *  - Frederico Rocha
 *
 * \section how2run How to run MinCAL
 * \subsection step1 Step 1: Creating the configuration file
 * You must copy the contents of the file <code>Conf/proj</code> to the directory <code>/etc/system.conf.d/</code>
 * \subsection step2 Step 2: Building the application
 * Change directory to <code>/pr/Src</code> and run <code>make</code>.
 * \subsection step3 Step 3: Running the service
 * Run the command <code>service run /pr/Src/proj -args init</code>, as root, in order to initialize the service.
 */

/** @defgroup init init
 * @{
 *
 * @brief The beginning of all.
 */


/**
 * @brief begin program execution
 * @param argv0 the argv[0] of the main function
 */
void *init(char* argv0);

/**}*/

#endif
