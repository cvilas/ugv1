/*
 * This is the top level Doxygen documentation file for this project. Use this 
 * file to setup the 'main page' and to define groupings for source code 
 * documentation.
 */

// The front page of your documentation goes here. Provide a description of your
// software here. You may also want to include a brief user's guide, instructions
// to compile and run the software, etc. Use \section and \subsection to organise 
// into sections. 

/**
 * \mainpage
 * 
 * \section secIntro Introduction
 * 
 */

// Define top level groupings for your source code documentation here. These
// will be listed in the 'Modules' section. 
// - use \defgroup to define a group. 
// - use \ingroup <name> in your source code to tell Doxygen you want that bit of
//   code documented in group <name>. Note that groups can be nested.
// - use \addtogroup anywhere in source code to define a new group if it doesn't
//   already exist.
// See http://www.doxygen.nl/grouping.html for more on grouping.

/**
 * \defgroup agent Agents
 * UGV agent nodes
 *
 * \defgroup agent_exceptions Agent Exceptions
 * \ingroup agent
 * Exceptions raised by agent nodes
 *
 * \defgroup controller Vehicle Controller
 * Vehicle controller
 *
 * \defgroup cont_comms Vehicle Controller Messages
 * \ingroup controller
 * Vehicle controller messages
 * 
 * \defgroup cont_exceptions Vehicle Controller Exceptions
 * \ingroup controller
 * Exceptions raised by vehicle controller
 *
 * \defgroup remote Remote
 * Remote systems
 *
 * \defgroup common Common
 * Generic/common functions
 * 
 */
