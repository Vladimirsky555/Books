#include "searchstorage.h"

SearchStorage::SearchStorage(QObject *parent) : QObject(parent){}

void SearchStorage::destroySearchList()
{
    for(int i = 0; i < searchItems.count(); i++)
    {
        delete searchItems[i];
    }
}

//Формирование элемента списка текстов, в которых есть хотя бы одно ключевое слово
void SearchStorage::addSearchItem(int cnt, Catalog *cat, BookItem *book,
                                  ListItem *chapter, TextItem *section)
{
    searchItem *item = new searchItem();
    item->_catalog = cat;
    item->_book = book;
    item->_chapter = chapter;
    item->_section = section;
    item->textCount = cnt;//количество повторений фразы
    searchItems.append(item);
}

void SearchStorage::sortResult()
{
    for(int i = 0; i < searchItems.count(); i++)
    {
        for(int j = 0; j < searchItems.count() - 1; j++)
        {
            if(searchItems[j]->textCount < searchItems[j + 1]->textCount)
            {
                searchItem *tmp = searchItems[j];
                searchItems[j] = searchItems[j + 1];
                searchItems[j + 1] = tmp;
            }
        }
    }
}





