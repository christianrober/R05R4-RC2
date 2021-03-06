// $Id: list-internal.h,v 1.1 2005/07/25 03:51:09 magicyang Exp $    --*- c -*--

// Copyright (C) 2005 Enrico Scholz <enrico.scholz@sigma-chemnitz.de>
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  

#ifndef H_UTILVSERVER_VECTOR_LIST_INTERNAL_H
#define H_UTILVSERVER_VECTOR_LIST_INTERNAL_H

struct ListItem
{
    void		*data;
    struct ListItem	*next;
};

struct ListItem *	List_insertInternal(struct List *list,
					    void const *data,
					    struct ListItem **before_pos,
					    struct ListItem *after_pos);

#endif	//  H_UTILVSERVER_VECTOR_LIST_INTERNAL_H
