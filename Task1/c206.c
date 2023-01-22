
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                  Implementace řešení: Kateřina Cibulcová (xcibul12), říjen 2020
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    //prochazim seznam a uvolnuji rusene prvky
    while (L->Act != NULL) {
        tDLElemPtr next = L->Act->rptr;
        free(L->Act);
        L->Act = next;
    }

    //inicializace seznamu
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokuji pamet pro novy prvek
    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    if (newElem == NULL) {
        DLError();
        return;
    }

    //nastavim prvku hodnotu val, levy ukazatel ukazuje na NULL
    newElem->data = val;
    newElem->lptr = NULL;

    if (L->First == NULL) {

        //pokud je vlozeny prvek prvni, nastavim i pravy ukazatel na NULL a ulozim
        newElem->rptr = NULL;
        L->First = newElem;
        L->Last = L->First;

    } else {
        //ulozim novy prvek misto prvniho prvku seznamu
        newElem->rptr = L->First;
        L->First->lptr = newElem;
        L->First = newElem;


    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokuji pamet pro novy prvek
    tDLElemPtr newElem = malloc(sizeof(struct tDLElem));

    if (newElem == NULL) {
        DLError();
        return;
    }

    //nastavim prvku hodnotu val, pravy ukazatel ukazuje na NULL
    newElem->data = val;
    newElem->rptr = NULL;

    if (L->Last == NULL) {

        //pokud je vlozeny prvek prvni, nastavim i levy ukazatel na NULL a ulozim
        newElem->lptr = NULL;
        L->Last = newElem;
        L->First = L->Last;
    } else {
        //ulozim novy prvek misto prvniho prvku seznamu
        L->Last->rptr = newElem;
        newElem->lptr = L->Last;
        L->Last = newElem;

    }

}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First == NULL) {
	    DLError();
	    return;
	} else {
	    *val = L->First->data;
	}

}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL) {
        DLError();
        return;
    } else {
        *val = L->Last->data;
    }

}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    //pri prazdnem seznamu se nic nedeje
    if (!L) {
        return;
    }

    if (L->First == NULL) {
        return;
    }

    //pokud je ruseny prvek aktivni, aktivita se ztraci
    if (L->First == L->Act) {
        L->Act = NULL;
    }

    //pokud je v seznamu pouze jeden prvek, uvolni se alokovana pamet a aktivita seznamu se ztraci
    if (L->Last == L->First) {
        L->First = NULL;
        L->Last = NULL;
        L->Act = NULL;
        free(L->Last);
    } else {

        //uvolni se alokovana pamet ruseneho prvku a prvnim prvkem se stava nasledujici prvek
        tDLElemPtr newFirst = L->First->rptr;
        free(L->First);
        L->First = newFirst;
        L->First->lptr = NULL;
        L->First->rptr = newFirst->rptr;
    }
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    //pri prazdnem seznamu se nic nedeje
    if (!L) {
        return;
    }

    if (L->Last == NULL) {
        return;
    }

    //pokud je ruseny prvek aktivni, aktivita se ztraci
    if (L->Last == L->Act) {
        L->Act = NULL;
    }

    //pokud je v seznamu pouze jeden prvek, uvolni se alokovana pamet a aktivita seznamu se ztraci
    if (L->Last == L->First) {
        L->First = NULL;
        L->Last = NULL;
        L->Act = NULL;
        free(L->Last);
    } else {
        //uvolni se alokovana pamet ruseneho prvku a poslednim prvkem se stava predchozi prvek
        tDLElemPtr newLast = L->Last->lptr;
        free(L->Last);
        L->Last = newLast;
        L->Last->rptr = NULL;
        L->Last->lptr = newLast->lptr;
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    //pri neaktivnim seznamu nebo aktivite na poslednim prvku se nic nedeje
    if(!L->Act){
        return;
    }

    if(L->Act->rptr == NULL) {
        return;

    } else if (L->Act->rptr == L->Last) {

        //pokud je ruseny prvek poslednim prvkem, poslednim prvkem se stava aktivni prvek
        L->Act->rptr = NULL;
        L->Last = L->Act;

        free(L->Act->rptr);
    } else {

        //rusim prvek za aktivnim prvkem a navazu aktivni prvek na nasledujici prvek
        tDLElemPtr nextElem = L->Act->rptr->rptr;
        free(L->Act->rptr);
        nextElem->lptr = L->Act;
        L->Act->rptr = nextElem;
    }
		
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    //pri neaktivnim seznamu nebo aktivite na poslednim prvku se nic nedeje
    if(!L->Act){
        return;
    }

    if(L->Act->lptr == NULL) {
        return;

    } else if (L->Act->lptr == L->First) {

        //pokud je ruseny prvek prvnim prvkem, prvnim prvkem se stava aktivni prvek
            L->Act->lptr = NULL;
            L->First = L->Act;
            free(L->Act->lptr);
        } else {

            //rusim prvek pred aktivnim prvkem a navazu aktivni prvek na predchozi prvek
            tDLElemPtr preElem = L->Act->lptr->lptr;
            free(L->Act->lptr);
            preElem->rptr = L->Act;
            L->Act->lptr = preElem;

        }

}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //pri neaktivnim seznamu se nic nedeje
    if (!L->Act) {
        return;
    }

    //alokuji pamet pro novy prvek
    tDLElemPtr newPostElem = malloc(sizeof(struct tDLElem));

    if (newPostElem == NULL) {
        DLError();
        return;
    }

    //nastavim hodnotu prvku, pravy ukazatel na prvek za aktivnim prvkem, levy ukazatel na aktivni prvek
    newPostElem->data = val;
    newPostElem->rptr = L->Act->rptr;
    newPostElem->lptr = L->Act;

    //pokud je aktivni prvek posledni, poslednim prvkem se stane novy prvek
    if (L->Act == L->Last) {

        L->Last = newPostElem;
    } else {
        //navazu novy prvek mezi aktivni a nasledujici prvek
        L->Act->rptr->lptr = newPostElem;
    }
    L->Act->rptr = newPostElem;

}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //pri neaktivnim seznamu se nic nedeje
    if (!L->Act) {
        return;
    }

    //alokuji pamet pro novy prvek
    tDLElemPtr newPreElem = malloc(sizeof(struct tDLElem));

    if (newPreElem == NULL) {
        DLError();
        return;
    }

    //nastavim hodnotu prvku, pravy ukazatel na aktivni prvek, levy ukazatel na prvek pred aktivnim prvkem
    newPreElem->data = val;
    newPreElem->lptr = L->Act->lptr;
    newPreElem->rptr = L->Act;

    if (L->Act == L->First) {
        //pokud je aktivni prvek prvni, prvnim prvkem se stane novy prvek
        L->First = newPreElem;
    } else {
        //navazu novy prvek mezi aktivni a predchozi prvek
        L->Act->lptr->rptr = newPreElem;

    }
    L->Act->lptr = newPreElem;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

    if (!L) {
        return;
    }
    if (!L->Act) {
        DLError();
        return;
    }

    *val = L->Act->data;
	
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if (!L) {
        return;
    }

    if(!L->Act) {
        return;
    }

    L->Act->data = val;
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
    if (!L) {
        return;
    }
    if (!L->Act) {
        return;
    }

    L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
    if (!L->Act) {
        return;
    }

    L->Act = L->Act->lptr;
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

    if (L->Act) {
        return 1;
    } else {
        return 0;
    }
}

/* Konec c206.c*/
