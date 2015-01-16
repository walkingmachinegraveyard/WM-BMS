/*
 * mlist.h
 *
 *  Created on: 2012-04-06
 *      Author: YLamarre
 */

#ifndef MLIST_H_
#define MLIST_H_
struct xlistNode{
	int value;					//Value used to sort list
	struct xlistNode *pNext;	//Next node
	void *pObject;				//Link to the contained object
};
typedef struct xlistNode MListNode;

struct xMList{
	int Length;
	struct xlistNode *StartNode;
	struct xlistNode *CurrentNode;
};
typedef struct xMList MList;

void InitMList(MList **pMList);

int GoToNodeValue(MList *pListHandle, int value);

void AddPackage(MList *pListHandle, int value, void *pObject);

void *ReturnThisPackage(MList *pListHandle);

void RemoveThisPackage(MList *pListHandle);

void *ReturnNextPackage(MList *pListHandle);

int MListLength(MList *pListHandle);

#endif /* MLIST_H_ */
