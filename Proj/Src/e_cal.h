#ifndef __TEST5_H
#define __TEST5_H

/**
 * @mainpage eCal
 * @section intro_sec Introduction
 * eCal is an application developed in the context of <a href="https://sigarra.up.pt/feup/pt/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=399888">LCOM</a>.
 * It depends heavily on device drivers developed specifically for this use.
 *
 * Authors:
 *  - Daniel Silva 		daniel.s@fe.up.pt
 *  - Frederico Rocha
 *
 * \section how2run How to run eCal
 * \subsection step1 Step 1: Creating the Symlink
 * Because of the way this program is loaded into memory, all paths must be absolute.
 * In order to facilitate reading auxiliary files, we require the creation of a soft link.
 * You can do this by running: <code>ln -s /path/to/your/working/copy /pr</code>
 * \subsection step2 Step 2: Creating the configuration file
 * You must copy the contents of the file <code>Conf/proj</code> to the directory <code>/etc/system.conf.d/</code>
 * \subsection step3 Step 3: Building the application
 * Change directory to <code>/pr/Src</code> and run <code>make</code>.
 * \subsection step4 Step 4: Running the service
 * Run the command <code>service run /pr/Src/proj -args init</code>, as root, in order to initialize the service.
 */

/** @defgroup init init
 * @{
 *
 * @brief The beginning of all.
 */


/**
 * @brief begin program execution
 */
void *init();

/**}*/

#endif
