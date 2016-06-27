/**
 * @file
 * $Revision$
 * $Date$
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are
 *   public domain. See individual third-party library and package descriptions
 *   for intellectual property information, user agreements, and related
 *   information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or
 *   implied, is made by the USGS as to the accuracy and functioning of such
 *   software and related material nor shall the fact of distribution
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */
#include "IsisDebug.h"

#include "ProjectItemProxyModel.h"

#include "ProjectItem.h"
#include "ProjectItemModel.h"

#include <QMimeData>

namespace Isis {
  /**
   * Constructs the proxy model.
   *
   * @param[in] parent (QObject *) The parent QObject.
   */
  ProjectItemProxyModel::ProjectItemProxyModel(QObject *parent) : ProjectItemModel(parent) {
    m_sourceModel = 0;
    m_sourceProxyMap = QMap<ProjectItem *, ProjectItem *>();
  }


  /**
   * Returns the QModelIndex of an item in the proxy model that corresponds with
   * the QModelIndex of an item in the source model.
   *
   * @param[in] sourceIndex (const QModelIndex &) The index from the
   *                                              source model.
   *
   * @return @b QModelIndex The index from the proxy model.
   */
  QModelIndex ProjectItemProxyModel::mapIndexFromSource(const QModelIndex &sourceIndex) {
    ProjectItem *proxyItem = mapItemFromSource( sourceModel()->itemFromIndex(sourceIndex) );
    
    if (proxyItem) {
      return proxyItem->index();
    }
    else {
      return QModelIndex();
    }

  }
  

  /**
   * Returns the QModelIndex of an item in the souce model that corresponds with
   * the QModelIndex of an item in the proxy model.
   *
   * @param[in] proxyIndex (const QModelIndex &) The index from the proxy model.
   *
   * @return @b QModelIndex The index from the source model.
   */
  QModelIndex ProjectItemProxyModel::mapIndexToSource(const QModelIndex &proxyIndex) {
    ProjectItem *sourceItem = mapItemToSource( itemFromIndex(proxyIndex) );
    
    if (sourceItem) {
      return sourceItem->index();
    }
    else {
      return QModelIndex();
    }
  }


  /**
   * Returns a QItemSelection of items in the proxy model that corresponds with
   * a QItemSelection of items in the source model.
   *
   * @param[in] sourceSelection (const QItemSelection &) The selection of items
   *                                                     in the source model.
   *
   * @return @b QItemSelection The selection of items in the proxy model.
   */
  QItemSelection ProjectItemProxyModel::mapSelectionFromSource(
      const QItemSelection &sourceSelection) {

    QItemSelection proxySelection = QItemSelection();

    foreach ( QModelIndex sourceIndex, sourceSelection.indexes() ) {
      QModelIndex proxyIndex = mapIndexFromSource(sourceIndex);
      if ( proxyIndex.isValid() ) {
        proxySelection.select(proxyIndex, proxyIndex);
      }
    }

    return proxySelection;
  }


  /**
   * Returns a QItemSelection of items in the source model that
   * corresponds with a QItemSelection of itesm in the proxy model.
   *
   * @param[in] proxySelection (const QItemSelection &) The selection of items
   *                                                    in the proxy model.
   *
   * @return @b QItemSelection The selection of items in the source model.
   */
  QItemSelection ProjectItemProxyModel::mapSelectionToSource(const QItemSelection &proxySelection) {
    QItemSelection sourceSelection = QItemSelection();
  
    foreach ( QModelIndex proxyIndex, proxySelection.indexes() ) {
      QModelIndex sourceIndex = mapIndexToSource(proxyIndex);
      if ( sourceIndex.isValid() ) {
        sourceSelection.select(sourceIndex, sourceIndex);
      }
    }

    return sourceSelection;
  }


  /**
   * Returns the ProjectItem in the proxy model that corresponds with a
   * ProjectItem in the source model.
   *
   * @param[in] sourceItem (ProjectItem *) The item in the source model.
   *
   * @return @b ProjectItem* The item in the proxy model.
   */
  ProjectItem *ProjectItemProxyModel::mapItemFromSource(ProjectItem *sourceItem) {
    return m_sourceProxyMap.value(sourceItem, 0);
  }


  /**
   * Returns the ProjectItem in the source model that corresponds with
   * a ProjectItem in the source model.
   *
   * @param[in] proxyItem (ProjectItem *) The item in the proxy model.
   *
   * @return @b ProjectItem* The item in the source model.
   */
  ProjectItem *ProjectItemProxyModel::mapItemToSource(ProjectItem *proxyItem) {
    return m_sourceProxyMap.key(proxyItem, 0);
  }


  /**
   * @brief Adds an item and its children to the proxy model.
   *
   * Creates items in the proxy model from an item in the source model and
   * returns the created item. If the item in the source model has already been
   * added, then the corresponding item in the proxy model is updated and
   * returned instead.
   *
   * When an item in the source model is added the children of that item are
   * also added as children of the corresponding item in the proxy model.
   *
   * @param[in] sourceItem (ProjectItem *) The item in the source model.
   *
   * @return @b ProjectItem* The item in the proxy model.
   */
  ProjectItem *ProjectItemProxyModel::addItem(ProjectItem *sourceItem) {
    if (!sourceItem) {
      return 0;
    }

    ProjectItem *proxyItem;
    
    if (ProjectItem *parentItem = mapItemFromSource( sourceItem->parent() ) ) {
      proxyItem = addChild(sourceItem, parentItem);
    }
    else {
      proxyItem = addChild(sourceItem, 0);
    }

    for (int i=0; i < sourceItem->rowCount(); i++) {
      addItem( sourceItem->child(i) );
    }

    return proxyItem;
  }


  /**
   * Adds a list of items to the proxy model.
   *
   * @param[in] sourceItem (QList<ProjectItem *>) The list of items in the
   *                                              source model.                                              
   */
  void ProjectItemProxyModel::addItems(QList<ProjectItem *> sourceItems) {
    foreach (ProjectItem *item, sourceItems) {
      addItem(item);
    }
  }

  
  /**
   * Removes an item and its children from the proxy model.
   *
   * @param[in] proxyItem (ProjectItem *) The item to be removed.
   */
  void ProjectItemProxyModel::removeItem(ProjectItem *item) {
    if (item) {
      m_sourceProxyMap.remove( mapItemToSource(item) );
    }
    ProjectItemModel::removeItem(item);
  }
  
  
  /**
   * Sets the source model. If there are already items in the model and the
   * source model is set to a different source model, the proxy model is
   * cleared first.
   *
   * @param[in] sourceModel (ProjectItemModel *) The source model.
   */
  void ProjectItemProxyModel::setSourceModel(ProjectItemModel *sourceModel) {
    if (m_sourceModel == sourceModel) {
      return;
    }

    if (m_sourceModel) {
      clear();
      m_sourceModel->disconnect(this);
    }

    m_sourceModel = sourceModel;

    connect(sourceModel->selectionModel(),
            SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &) ),
            this,
            SLOT( updateProxyCurrent() ) );

    connect(selectionModel(),
            SIGNAL( currentChanged(const QModelIndex &, const QModelIndex &) ),
            this,
            SLOT( updateSourceCurrent() ) );

    connect(sourceModel->selectionModel(),
            SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection &) ),
            this,
            SLOT( updateProxySelection() ) );
    
    connect(selectionModel(),
            SIGNAL( selectionChanged(const QItemSelection &, const QItemSelection &) ),
            this,
            SLOT( updateSourceSelection() ) );
    connect(sourceModel, SIGNAL( itemChanged(QStandardItem *) ),
            this, SLOT( onItemChanged(QStandardItem *) ) );
  }


  /**
   * Returns the source model.
   *
   * @return @b ProjectItemModel* The source model.
   */
  ProjectItemModel *ProjectItemProxyModel::sourceModel() {
    return m_sourceModel;
  }


  /**
   * Given an item in the source model, this method changes the data
   * of the corresponding item in the proxy model to match that of the
   * item in the source model.
   *
   * @param[in] sourceItem (ProjectItem *) The item in the source model.
   */
  void ProjectItemProxyModel::updateItem(ProjectItem *sourceItem) {
    if ( ProjectItem *proxyItem = mapItemFromSource(sourceItem) ) {
      proxyItem->setProjectItem(sourceItem);
    }
  }

  
  /**
   * Slot that updates the current item in the proxy model only if it is
   * different than the corresponding item in the source model.
   */
  void ProjectItemProxyModel::updateProxyCurrent() {
    QModelIndex newProxyCurrent = mapIndexFromSource( 
        sourceModel()->selectionModel()->currentIndex() );
    if ( newProxyCurrent != selectionModel()->currentIndex() ) {
      selectionModel()->setCurrentIndex(newProxyCurrent, QItemSelectionModel::Current);
    }
  }


  /**
   * Slot that updates the current item in the proxy model only if it is
   * different than the corresponding item in the source model.
   */
  void ProjectItemProxyModel::updateSourceCurrent() {
    QModelIndex newSourceCurrent = mapIndexToSource( selectionModel()->currentIndex() );
    if ( newSourceCurrent != sourceModel()->selectionModel()->currentIndex() ) {
      sourceModel()->selectionModel()->setCurrentIndex(newSourceCurrent, 
                                                       QItemSelectionModel::Current);
    }
  }


  /**
   * Slot that updates the selection in the proxy model only if it is
   * different than the corresponding selection in the source model.
   */
  void ProjectItemProxyModel::updateProxySelection() {
    QItemSelection newProxySelection = mapSelectionFromSource( 
        sourceModel()->selectionModel()->selection() );
    if ( newProxySelection != selectionModel()->selection() ) {
      selectionModel()->select(newProxySelection, QItemSelectionModel::ClearAndSelect);
    }
  }
 

  /**
   * Slot that updates the selection in the source model only if it is
   * different than the corresponding selection in the proxy model.
   */
  void ProjectItemProxyModel::updateSourceSelection() {
    QItemSelection newSourceSelection = mapSelectionToSource( selectionModel()->selection() );
    if ( mapSelectionFromSource(newSourceSelection) != 
         mapSelectionFromSource( sourceModel()->selectionModel()->selection() ) ) {
      sourceModel()->selectionModel()->select(newSourceSelection, 
                                              QItemSelectionModel::ClearAndSelect);
    }
  }


  /**
   * Creates an item in the proxy model corresponding to an item in the
   * source model as a child of a parent item in the proxy model. If the
   * corresponding item in the proxy model already exits then that item is
   * updated instead of creating a new item. If the corresponding item already
   * has a parent its parent is changed to the new parent. If the parent item
   * is null then the item is added as a top-level item in the model.
   *
   * @param[in] sourceItem (ProjectItem *) The item in the source model.
   * @param[in] parentItem (ProjectItem *) The parent in the proxy model.
   *
   * @return @b ProjectItem* The correponding item in the proxy model.
   */
  ProjectItem *ProjectItemProxyModel::addChild(ProjectItem *sourceItem, ProjectItem *parentItem) {
    if (!sourceItem) {
      return 0;
    }

    if ( !sourceModel() ) {
      setSourceModel( sourceItem->model() );
    }

    if ( sourceItem->model() != sourceModel() ) {
      return 0;
    }

    if ( parentItem && (parentItem->model() != this) ) {
      return 0;
    }
    
    ProjectItem *proxyItem = mapItemFromSource(sourceItem);
    
    if (!proxyItem) {
      proxyItem = new ProjectItem();
      proxyItem->setProjectItem(sourceItem);
      m_sourceProxyMap.insert(sourceItem, proxyItem);
    }
    else {
      if ( ProjectItem *oldParent = proxyItem->parent() ) {
        oldParent->takeRow( proxyItem->row() );
      }
      else if ( ProjectItemModel *model = proxyItem->model() ) {
        model->takeRow( proxyItem->row() );
      }
    }

    if (parentItem) {
      parentItem->appendRow(proxyItem);
    }
    else {
      appendRow(proxyItem);
    }

    return proxyItem;
  }


  /**
   * Signal to connect to the itemChanged() signal from a
   * ProjectItemModel. Updates the corresponding item in the proxy
   * model.
   *
   * @param[in] item (QStandardItem *) The item that has been changed.
   */
  void ProjectItemProxyModel::onItemChanged(QStandardItem *item) {
    updateItem( static_cast<ProjectItem *>(item) );
  }


  /**
   * Returns true. You can drop data into a ProjectItemProxyModel
   *
   * @param data (const QMimeData *) The data to drop
   * @param action (Qt::DropAction) The drop action
   * @param row (int) ???
   * @param column(int) ???
   * @param QModelIndex (const QModelIndex &) Index of the data's parent item
   *
   * @return @b bool True if the proxy model can accept the mime data.
   */
  bool ProjectItemProxyModel::canDropMimeData(const QMimeData *data,
                                           Qt::DropAction action,
                                           int row, int column,
                                           const QModelIndex &parent) const {
    return true;
  }

  
  /**
   * Adds the data (selected items) from the source model to the proxy model.
   *
   * @param data (const QMimeData *)
   * @param action (Qt::DropAction)
   * @param row (int)
   * @param column(int)
   * @param QModelIndex (const QModelIndex &) 
   *
   * @return @b bool True if the data was successfully added to the proxy model.
   */
  bool ProjectItemProxyModel::dropMimeData(const QMimeData *data,
                                           Qt::DropAction action,
                                           int row, int column,
                                           const QModelIndex &parent) {
    if ( data->hasFormat("application/x-qabstractitemmodeldatalist") ) {
      addItems( sourceModel()->selectedItems() );
      return true;
    }
    return false;
  }
}
