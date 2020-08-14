/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#ifndef LISTH
#define LISTH
/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

struct list_t {
	struct list_t *next;
};

static inline struct list_t *list_add(struct list_t **_prev, struct list_t *_new)
{
	if ((*_prev) == (void *)0) {
		_new->next = 0;
		*_prev = _new;
	} else {
		_new->next = (*_prev)->next;
		(*_prev)->next = _new;
	}
	return _new;
}

static inline struct list_t *list_remove(struct list_t *_prev, struct list_t *_remove)
{
	_prev->next = _remove->next;
	return _remove;
}

#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif /* LISTH */
