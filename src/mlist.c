/*
 * mlist.c
 *
 *  Created on: 2012-04-06
 *      Author: YLamarre
 */

#include "FreeRTOS.h"
#include "mlist.h"

MListNode *ReachNode(int value, MListNode *pLastNode);

void InitMList(MList **ppMList)
{
	//
	if(*ppMList == NULL)
		*ppMList = (MList *) pvPortMalloc(sizeof(MList));
	(*ppMList)->CurrentNode = NULL;
	(*ppMList)->StartNode = NULL;
	(*ppMList)->Length = 0;
}

int GoToNodeValue(MList *pListHandle, int value)
{
	if(pListHandle->CurrentNode != NULL)
	{
		if(pListHandle->CurrentNode->value < value)
			pListHandle->CurrentNode = ReachNode(value,pListHandle->CurrentNode);
		else
			pListHandle->CurrentNode = ReachNode(value,pListHandle->StartNode);
		return pListHandle->CurrentNode->value;
	}
	else
		return 0;
}

void AddPackage(MList *pListHandle, int value, void *pObject)
{
	//
	MListNode *NewPackage;
	if(pListHandle->Length > 0)
	{
		GoToNodeValue(pListHandle, value);
		if(pListHandle->CurrentNode->value != value)
		{
			NewPackage = pvPortMalloc(sizeof(MListNode));
			NewPackage->pNext = pListHandle->CurrentNode->pNext;
			pListHandle->CurrentNode->pNext = NewPackage;
		}
		//TODO : FIX THIS!!!!
		else
			NewPackage = pListHandle->CurrentNode; //Yeah, so if the package is already in the list,
												   //the package is overwritten but the length is still growing.
	}
	else
	{
		NewPackage = pvPortMalloc(sizeof(MListNode));
		NewPackage->pNext=NULL;
		pListHandle->CurrentNode=NewPackage;
		pListHandle->StartNode=NewPackage;
	}
	NewPackage->value = value;
	NewPackage->pObject = pObject;
	pListHandle->Length++;			//Should not update if already in the list.
}

void *ReturnThisPackage(MList *pListHandle)
{
	return pListHandle->CurrentNode->pObject;
}

void *ReturnNextPackage(MList *pListHandle)
{
	void *retValue;
	if(pListHandle->CurrentNode->pNext != NULL)
	{
		pListHandle->CurrentNode = pListHandle->CurrentNode->pNext;
		retValue = pListHandle->CurrentNode->pObject;
	}
	else
		retValue = NULL;
	return retValue;
}

void RemoveThisPackage(MList *pListHandle)
{

	if(pListHandle->Length > 1)
	{
		MListNode *SwapNode;
		SwapNode = ReachNode(pListHandle->CurrentNode->value - 1, pListHandle->StartNode);
		SwapNode->pNext = pListHandle->CurrentNode->pNext;
		vPortFree(pListHandle->CurrentNode);
		pListHandle->CurrentNode = SwapNode;
	}
	else if(pListHandle->Length == 1)
	{
		vPortFree(pListHandle->CurrentNode);
		InitMList(&pListHandle);
	}
}

MListNode *ReachNode(int value, MListNode *pLastNode)
{
	if(pLastNode->pNext != NULL)
		if(pLastNode->pNext->value <= value)
			return ReachNode(value, pLastNode->pNext);
	return pLastNode;
}

int MListLength(MList *pListHandle)
{
	return pListHandle->Length;
}
