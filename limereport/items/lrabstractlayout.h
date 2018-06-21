#ifndef LRABSTRACTLAYOUT_H
#define LRABSTRACTLAYOUT_H

#include "lritemdesignintf.h"
#include "lrlayoutmarker.h"

namespace LimeReport{
class AbstractLayout: public LayoutDesignIntf
{
    Q_OBJECT
    Q_ENUMS(LayoutType)
    Q_PROPERTY(bool hideEmptyItems READ hideEmptyItems WRITE setHideEmptyItems)
public:
    enum LayoutType{Layout,Table};
    AbstractLayout(QString xmlTag, QObject *owner = 0, QGraphicsItem *parent = 0);
    ~AbstractLayout();
    QList<BaseDesignIntf*>& layoutsChildren();
    LayoutMarker* layoutMarker() const;
    bool isRelocating() const;
    void setIsRelocating(bool isRelocating);
    LayoutType layoutType() const;
    void setLayoutType(const LayoutType& layoutType);

    void addChild(BaseDesignIntf *item,bool updateSize=true);
    void restoreChild(BaseDesignIntf *item);
    bool isEmpty() const;
    void paint(QPainter* ppainter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    bool hideEmptyItems() const;
    void setHideEmptyItems(bool hideEmptyItems);

protected:
    int  childrenCount();
    void beforeDelete();
    void childAddedEvent(BaseDesignIntf *child);
    void geometryChangedEvent(QRectF newRect, QRectF);
    void initMode(ItemMode mode);
    void setBorderLinesFlags(BorderLines flags);
    void collectionLoadFinished(const QString &collectionName);
    void objectLoadFinished();
    bool isNeedUpdateSize(RenderPass pass) const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void updateItemSize(DataSourceManager* dataManager, RenderPass pass, int maxHeight);
private:
    virtual void divideSpace() = 0;
    virtual void updateLayoutSize() = 0;
    virtual void relocateChildren() = 0;
    virtual BaseDesignIntf *findNext(BaseDesignIntf *item) = 0;
    virtual BaseDesignIntf *findPrior(BaseDesignIntf *item) = 0;
    virtual void placeItemInLayout(BaseDesignIntf* item) = 0;
    virtual void insertItemInLayout(BaseDesignIntf* item) = 0;
private slots:
    void slotOnChildDestroy(QObject *child);
    void slotOnChildGeometryChanged(QObject*item, QRectF newGeometry, QRectF oldGeometry);
    void slotOnChildItemAlignChanged(BaseDesignIntf* item, const ItemAlign&, const ItemAlign&);
    void slotOnChildVisibleHasChanged(BaseDesignIntf*);
    void slotOnChildSelectionHasChanged(BaseDesignIntf* item, bool value);
private:
    QList<BaseDesignIntf *> m_children;
    bool m_isRelocating;
    LayoutMarker* m_layoutMarker;
    LayoutType m_layoutType;
    bool m_hideEmptyItems;
};

} // namespace LimeReport

#endif // LRABSTRACTLAYOUT_H
