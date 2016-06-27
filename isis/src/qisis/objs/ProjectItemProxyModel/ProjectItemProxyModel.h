#ifndef ProjectItemProxyModel_h
#define ProjectItemProxyModel_h
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

#include <QIdentityProxyModel>
#include <QItemSelection>
#include <QMap>

#include "ProjectItem.h"
#include "ProjectItemModel.h"

namespace Isis {
  /**
   * Allows access to items in a ProjectItemModel through a proxy
   * model. A proxy model can have a different structure than the
   * source model. An item in the proxy model usually corresponds to
   * an item in the source model. The proxy model will update item
   * selections and the current item between the source model and the
   * proxy model.
   *
   * In the default implementation the only items in the proxy model
   * are item that are added with the addItem() method. The items that
   * are added are organized in the same tree structure that they are
   * in the source model. Subclasses of ProjectItemProxyModel can
   * organize items in a different way by overriding the addItem()
   * method.
   *
   * The proxy model ensures that its item selection corresponds to
   * the item selection in the source model. When the selection in the
   * proxy model changes the selection in the source model is changed
   * to the items that correspond to the items in the proxy
   * selection. Similarly when the selection in the source model is
   * changed the selection in the proxy model is changed to the items
   * in the proxy model that correpond to the items in the selection.
   *
   * @code
   * ProjectItemModel *model = new ProjectItemModel(this);
   * model->addProject(project);
   * ProjectItemProxyModel *proxyModel = new ProjectItemProxyModel(this);
   * proxyModel->setSourceModel(model);
   * for (int i=0; i < model.rowCount(); i++) {
   *   proxyModel->addItem( model->item(i) );
   * }
   * @endcode
   *
   * @internal
   *   @author Jeffrey Covington
   *
   *   @history 2015-10-21 Jeffrey Covington - Original version.
   *   @history 2016-01-13 Jeffrey Covington - Added canDropMimeData() and dropMimeData() methods.
   *   @history 2016-06-27 Ian Humphrey - Added documentation to the canDropMimeData() and 
   *                           dropMimeData() methods. Checked coding standards. Fixes #4006.
   */
  class ProjectItemProxyModel : public ProjectItemModel {

  Q_OBJECT
  
  public:
    ProjectItemProxyModel(QObject *parent = 0);
    
    QModelIndex mapIndexFromSource(const QModelIndex &sourceIndex);
    QModelIndex mapIndexToSource(const QModelIndex &proxyIndex);

    QItemSelection mapSelectionFromSource(const QItemSelection &sourceSelection);
    QItemSelection mapSelectionToSource(const QItemSelection &proxySelection);

    ProjectItem *mapItemFromSource(ProjectItem *sourceItem);
    ProjectItem *mapItemToSource(ProjectItem *proxyItem);
    
    void removeItem(ProjectItem *item);

    void setSourceModel(ProjectItemModel *sourceModel);
    ProjectItemModel *sourceModel();

    virtual bool canDropMimeData(const QMimeData * data, Qt::DropAction action,
                                 int row, int column, const QModelIndex & parent) const;
    virtual bool dropMimeData(const QMimeData * data, Qt::DropAction action,
                                 int row, int column, const QModelIndex & parent);
  
  public slots:
    ProjectItem *addItem(ProjectItem *sourceItem);
    void addItems(QList<ProjectItem *> sourceItems);

  protected slots:
    void updateItem(ProjectItem *sourceItem);
    void updateProxyCurrent();
    void updateSourceCurrent();
    void updateProxySelection();
    void updateSourceSelection();

  protected:
    ProjectItem *addChild(ProjectItem *sourceItem, ProjectItem *parentItem);

  private slots:
    void onItemChanged(QStandardItem *item);
    
  private:
    ProjectItemModel *m_sourceModel; //!< The source model.
    //! Map of items from the source model to the proxy model.
    QMap<ProjectItem *, ProjectItem *> m_sourceProxyMap;
  };
}
#endif
