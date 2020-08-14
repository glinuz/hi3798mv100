/*******************************************************************************
* Copyright @ Huawei Technologies Co., Ltd. 1998-2014. All rights reserved.
* File name: strtok_s.c
* History:
*     1. Date:
*         Author:
*         Modification:
********************************************************************************
*/

#include "securec.h"

/*******************************************************************************
 * <NAME>
 *    strtok_s
 *
 * <SYNOPSIS>
 *    char* strtok_s(char* strToken, const char* strDelimit, char** context);
 *
 * <FUNCTION DESCRIPTION>
 *    The strtok_s function finds the next token in strToken.
 *
 * <INPUT PARAMETERS>
 *    strToken            String containing token or tokens.
 *    strDelimit          Set of delimiter characters.
 *    context             Used to store position information between calls
 *                        to strtok_s
 *
 * <OUTPUT PARAMETERS>
 *
 * <RETURN VALUE>
 *    Returns a pointer to the next token found in strToken.
 *    They return NULL when no more tokens are found.
 *    Each call modifies strToken by substituting a NULL character for the first
 *    delimiter that occurs after the returned token.
 *
 *    return value        condition
 *    NULL                context == NULL, strDelimit == NULL, strToken == NULL
 *                        && (*context) == NULL, or no token is found.
 *******************************************************************************
*/


char *strtok_s(char *strToken, const char *strDelimit, char **context)
{
	char *token = NULL;
	const char *ctl = NULL;

	/* validate delimiter and string context */
	if (context == NULL || strDelimit == NULL) {
		return NULL;
	}

	/*valid input string and string pointer from where to search*/
	if (strToken == NULL && (*context) == NULL) {
		return NULL;
	}

	/* If string is null, continue searching from previous string position stored in context*/
	if (NULL == strToken) {
		strToken = *context;
	}

	/* Find beginning of token (skip over leading delimiters). Note that
	* there is no token if this loop sets string to point to the terminal null.
	*/
	while (*strToken != 0) {
		ctl = strDelimit;
		while (*ctl != 0 && *ctl != *strToken) {
			++ctl;
		}

		if (*ctl == 0) { /*don't find any delimiter in string header, break the loop*/
			break;
		}
		++strToken;
	}

	token = strToken; /*point to updated position*/

	/* Find the rest of the token. If it is not the end of the string,
	* put a null there.
	*/
	for (; *strToken != 0 ; strToken++) {
		for (ctl = strDelimit; *ctl != 0 && *ctl != *strToken; ctl++)
			;
		if (*ctl != 0) { /*find a delimiter*/
			*strToken++ = 0; /*set string termintor*/
			break;
		}
	}

	/* record string position for next search in the context */
	*context = strToken;

	/* Determine if a token has been found. */
	if (token == strToken) {
		return NULL;
	} else {
		return token;
	}
}


