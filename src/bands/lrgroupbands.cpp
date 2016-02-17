/***************************************************************************
 *   This file is part of the Lime Report project                          *
 *   Copyright (C) 2015 by Alexander Arin                                  *
 *   arin_a@bk.ru                                                          *
 *                                                                         *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      *
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/
#include "lrgroupbands.h"
#include "lrglobal.h"
#include "lrdatasourcemanager.h"


const QString xmlTagHeader = QLatin1String("GroupHeader");
const QString xmlTagFooter = QLatin1String("GroupFooter");

namespace{

LimeReport::BaseDesignIntf* createHeader(QObject* owner, LimeReport::BaseDesignIntf*  parent){
    return new LimeReport::GroupBandHeader(owner,parent);
}

bool registredHeader = LimeReport::DesignElementsFactory::instance().registerCreator(
       xmlTagHeader,
        LimeReport::ItemAttribs(QObject::tr("GroupHeader"),LimeReport::Const::bandTAG),
        createHeader
    );

LimeReport::BaseDesignIntf * createFooter(QObject* owner, LimeReport::BaseDesignIntf*  parent){
    return new LimeReport::GroupBandFooter(owner,parent);
}

bool registredFooter = LimeReport::DesignElementsFactory::instance().registerCreator(
        xmlTagFooter,
        LimeReport::ItemAttribs(QObject::tr("GroupFooter"),LimeReport::Const::bandTAG),
        createFooter
    );

}

namespace LimeReport{

GroupBandHeader::GroupBandHeader(QObject *owner, QGraphicsItem *parent)
    : BandDesignIntf(BandDesignIntf::GroupHeader, xmlTagHeader, owner,parent),
      m_groupFiledName(""), m_groupStarted(false)
{
    setBandTypeText(tr("GroupHeader"));
    setFixedPos(false);
    setMarkerColor(bandColor());
}

bool GroupBandHeader::isUnique() const
{
    return false;
}

//bool GroupBandHeader::tryToKeepTogether()
//{
//    return m_tryToKeepTogether;
//}

BaseDesignIntf *GroupBandHeader::createSameTypeItem(QObject *owner, QGraphicsItem *parent)
{
    return new GroupBandHeader(owner, parent);
}

void GroupBandHeader::startGroup()
{
    m_groupStarted=true;

    DataSourceManager* dm = DataSourceManager::instance();
    QString lineVar = QLatin1String("line_")+objectName().toLower();
    dm->setReportVariable(lineVar,1);

    if ((dm->dataSource(parentBand()->datasourceName()))){
        IDataSource* ds = dm->dataSource(parentBand()->datasourceName());
        if (ds->columnIndexByName(m_groupFiledName)!=-1)
            m_groupFieldValue=ds->data(m_groupFiledName);
    }
}

QColor GroupBandHeader::bandColor() const
{
    return QColor(Qt::darkBlue);
}

bool GroupBandHeader::isNeedToClose()
{
    //if (m_groupFieldValue.isNull()) return false;

    if (!m_groupStarted) return false;
    DataSourceManager* dm = DataSourceManager::instance(); 
    if (m_groupFiledName.isNull() || m_groupFiledName.isEmpty())
        dm->putError("Group Field Not found");
    if ((dm->dataSource(parentBand()->datasourceName()))){
        IDataSource* ds = dm->dataSource(parentBand()->datasourceName());
        if (ds->data(m_groupFiledName).isNull() && m_groupFieldValue.isNull()) return false;
        return ds->data(m_groupFiledName)!=m_groupFieldValue;
    }

    return false;
}

bool GroupBandHeader::isStarted()
{
    return m_groupStarted;//!m_groupFieldValue.isNull();
}

void GroupBandHeader::closeGroup()
{
    m_groupFieldValue=QVariant();
    m_groupStarted=false;
}

int GroupBandHeader::index()
{
    return bandIndex();
}

GroupBandFooter::GroupBandFooter(QObject *owner, QGraphicsItem *parent)
    :BandDesignIntf(BandDesignIntf::GroupFooter, xmlTagFooter, owner,parent)
{
    setBandTypeText(tr("GroupFooter"));
    setFixedPos(false);
    setMarkerColor(bandColor());
}

bool GroupBandFooter::isUnique() const
{
    return false;
}

QColor GroupBandFooter::bandColor() const
{
    return QColor(Qt::darkBlue);
}

BaseDesignIntf *GroupBandFooter::createSameTypeItem(QObject *owner, QGraphicsItem *parent)
{
    return new GroupBandFooter(owner,parent);
}

} //namespace LimeReport
