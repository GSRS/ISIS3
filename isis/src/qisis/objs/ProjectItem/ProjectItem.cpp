/**
 * @file
 * $Date$
 * $Revision$
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

#include "ProjectItem.h"

#include "BundleResults.h"
#include "BundleSettings.h"
#include "BundleSolutionInfo.h"
#include "Control.h"
#include "ControlList.h"
#include "CorrelationMatrix.h"
#include "Image.h"
#include "ImageList.h"
#include "Project.h"
#include "ProjectItemModel.h"

namespace Isis {
  /**
   * Constructs an item without children, a parent, or a model.
   */
  ProjectItem::ProjectItem() : QStandardItem() {}

  
  /**
   * Contructs a copy of another item. The copy will have the same text,
   * icon, and data, and copies of the children. The copy will not have a
   * parent or a model.
   * 
   * @param[in] item (ProjectItem *) The item to copy.
   */
  ProjectItem::ProjectItem(ProjectItem *item) {
    setProjectItem(item);
    for (int i=0; i < item->rowCount(); i++) {
      appendRow(new ProjectItem( item->child(i) ) );
    }
  }


  /**
   * Constructs an item from a BundleResults.
   *
   * @param[in] bundleResults (BundleResults) The BundleResults to
   *                                          construct from.
   */
  ProjectItem::ProjectItem(BundleResults bundleResults) {
    setBundleResults(bundleResults);
    appendRow( new ProjectItem( bundleResults.correlationMatrix() ) );
  }


  /**
   * Constructs an item from a BundleSettings.
   *
   * @param[in] bundleSettings (BundleSettingsQsp) The BundleSettings to
   *                                               construct from.
   */
  ProjectItem::ProjectItem(BundleSettingsQsp bundleSettings) {
    setBundleSettings(bundleSettings);
  }


  /**
   * Constructs an item from a BundleSolutionInfo.
   *
   * @param[in] bundleSolutionInfo (BundleSolutionInfo *) The BundleSolutionInfo
   *                                                      to construct from.
   */
  ProjectItem::ProjectItem(BundleSolutionInfo *bundleSolutionInfo) {
    setBundleSolutionInfo(bundleSolutionInfo);
    
    QString cNetFileName = bundleSolutionInfo->controlNetworkFileName();
    Control *control = new Control(cNetFileName);
    appendRow( new ProjectItem(control) );

    appendRow( new ProjectItem( bundleSolutionInfo->bundleSettings() ) );
    appendRow( new ProjectItem( bundleSolutionInfo->bundleResults() ) );
  }


  /**
   * Constructs an item from a Control.
   *
   * @param[in] control (Control *) The Control to construct from.
   */
  ProjectItem::ProjectItem(Control *control) {
    setControl(control);
  }


  /**
   * Constructs an item from a ControlList.
   *
   * @param[in] controlList (ControlList *) The ControlList to construct from.
   */
  ProjectItem::ProjectItem(ControlList *controlList) {
    setControlList(controlList);
    foreach (Control *control, *controlList) {
      appendRow( new ProjectItem(control) );
    }
  }


  /**
   * Constructs an item from a list of ControlList.
   *
   * @param[in] controls (QList<ControlList *>) The list to construct from.
   */
  ProjectItem::ProjectItem(QList<ControlList *> controls) {
    setControls();
    foreach (ControlList *controlList, controls) {
      appendRow( new ProjectItem(controlList) );
    }
  }


  /**
   * Constructs an item from a CorrelationMatrix.
   *
   * @param[in] correlationMatrix (CorrelationMatrix) The CorrelationMatrix to
   *                                                  construct from.
   */
  ProjectItem::ProjectItem(CorrelationMatrix correlationMatrix) {
    setCorrelationMatrix(correlationMatrix);
  }


  /**
   * Constructs an item from an Image.
   *
   * @param[in] image (Image *) The Image to construct from.
   */
  ProjectItem::ProjectItem(Image *image) {
    setImage(image);
  }


  /**
   * Constructs an item from an ImageList.
   *
   * @param[in] imageList (ImageList *) The ImageList to construct from.
   */
  ProjectItem::ProjectItem(ImageList *imageList) {
    setImageList(imageList);
    foreach (Image *image, *imageList) {
      appendRow( new ProjectItem(image) );
    }
  }


  /**
   * Constructs an item from a list of ImageList.
   *
   * @param[in] images (QList<ImageList *>) The list to construct from.
   */
  ProjectItem::ProjectItem(QList<ImageList *> images) {
    setImages();
    foreach (ImageList *imageList, images) {
      appendRow( new ProjectItem(imageList) );
    }
  }

  
  /**
   * Constructs an item from a GuiCameraQsp
   *
   * @param[in] guiCamera (GuiCameraQsp) The camera to construct from.
   */
  ProjectItem::ProjectItem(GuiCameraQsp guiCamera) {
    setGuiCamera(guiCamera);
  }

  
  /**
   * Constructs an item from a GuiCameraList.
   *
   * @param[in] guiCameraList (GuiCameraList *) The list of cameras to
   *                                            construct from.
   */
  ProjectItem::ProjectItem(GuiCameraList *guiCameraList) {
    setGuiCameraList();
    foreach (GuiCameraQsp guiCamera, *guiCameraList) {
      appendRow( new ProjectItem(guiCamera) );
    }
  }


  /**
   * Constructs an item from a Project.
   *
   * @param[in] project (Project *) The Project to construct from.
   */
  ProjectItem::ProjectItem(Project *project) {
    setProject(project);

    appendRow( new ProjectItem( project->controls() ) );
    appendRow( new ProjectItem( project->images() ) );
    
    ProjectItem *shapeModelsItem = new ProjectItem();
    shapeModelsItem->setShapeModels();
    appendRow(shapeModelsItem);

    ProjectItem *targetBodyListItem = new ProjectItem();
    targetBodyListItem->setTargetBodyList();
    appendRow(targetBodyListItem);
    
    ProjectItem *guiCameraListItem = new ProjectItem();
    guiCameraListItem->setGuiCameraList();
    appendRow(guiCameraListItem);
    
    ProjectItem *spaceCraftItem = new ProjectItem();
    spaceCraftItem->setSpacecraft();
    appendRow(spaceCraftItem);
    
    appendRow( new ProjectItem( project->bundleSolutionInfo() ) );
  }


  /**
   * Constructs an item from a list of BundleSolutionInfo.
   *
   * @param[in] results (QList<BundleSolutionInfo *>) The list to construct
   *                                                  from.
   */
  ProjectItem::ProjectItem(QList<BundleSolutionInfo *> results) {
    setResults();
    foreach (BundleSolutionInfo *bundleSolutionInfo, results) {
      appendRow( new ProjectItem( bundleSolutionInfo) );
    }
  }


  /**
   * Constructs an item from a TargetBodyQsp.
   *
   * @param[in] targetBody (TargetBodyQsp) The target body to construct from.
   */
  ProjectItem::ProjectItem(TargetBodyQsp targetBody) {
    setTargetBody(targetBody);
  }


  /**
   * Constructs an item from a TargetBodyList.
   *
   * @param[in] targetBodyList (TargetBodyList *) The list to construct from.
   */
  ProjectItem::ProjectItem(TargetBodyList *targetBodyList) {
    setTargetBodyList();
    foreach (TargetBodyQsp targetBody, *targetBodyList) {
      appendRow( new ProjectItem(targetBody) );
    }
  }


  /**
   * Destructs a ProjectItem.
   */
  ProjectItem::~ProjectItem() {}


  /**
   * Returns the BundleResults stored in the data of the item.
   *
   * @return @b BundleResults The BundleResults of the item.
   */
  BundleResults ProjectItem::bundleResults() const {
    return data().value<BundleResults>();
  }


  /**
   * Returns the BundleSettings stored in the data of the item.
   *
   * @return @b BundleSettings* The BundleSettings of the item.
   */
  BundleSettingsQsp ProjectItem::bundleSettings() const {
    return data().value<BundleSettingsQsp>();
  }


  /**
   * Returns the BundleSolutionInfo stored in the data of the item.
   *
   * @return @b BundleSolutionInfo* The BundleSolutionInfo of the item.
   */
  BundleSolutionInfo *ProjectItem::bundleSolutionInfo() const {
    return data().value<BundleSolutionInfo *>();
  }


  /**
   * Returns the Image stored in the data of the item.
   *
   * @return @b Image* The Image of the item.
   */
  Image *ProjectItem::image() const {
    return data().value<Image *>();
  }


  /**
   * Returns the ImageList stored in the data of the item.
   *
   * @return @b ImageList* The ImageList of the item.
   */
  ImageList *ProjectItem::imageList() const {
    return data().value<ImageList *>();
  }


  /**
   * Returns the Control stored in the data of the item.
   *
   * @return @b Control* The Control of the item.
   */
  Control *ProjectItem::control() const {
    return data().value<Control *>();
  }


  /**
   * Returns the ControlList stored in the data of the item.
   *
   * @return @b ControlList* The ControlList of the item.
   */
  ControlList *ProjectItem::controlList() const {
    return data().value<ControlList *>();
  }

  
  /**
   * Returns the CorrelationMatrix stored the item.
   *
   * @return @b CorrelationMatrix* The CorrelationMatrix of the item.
   */
  CorrelationMatrix ProjectItem::correlationMatrix() const {
    return data().value<CorrelationMatrix>();
  }


  /**
   * Returns the Project stored in the data of the item.
   *
   * @return @b Project* The Project of the item.
   */
  Project *ProjectItem::project() const {
    return data().value<Project *>();
  }


  /**
   * Returns the GuiCameraQsp stored in the data of the item.
   *
   * @return @b GuiCameraQsp The camera stored in the item.
   */
  GuiCameraQsp ProjectItem::guiCamera() const {
    return data().value<GuiCameraQsp>();

  }
  

  /**
   * Returns the TargetBodyQsp stored in the data of the item.
   * 
   * @return @b TargetBodyQsp The target body stored in the item.
   */
  TargetBodyQsp ProjectItem::targetBody() const {
    return data().value<TargetBodyQsp>();
  }


  /**
   * Returns true if BundleResults are stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If BundleResults are stored in the data of the item or not.
   */
  bool ProjectItem::isBundleResults() const {
    return data().canConvert<BundleResults>();
  }


  /**
   * Returns true if BundleSettings are stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If BundleSettings are stored in the data of the item or not.
   */
  bool ProjectItem::isBundleSettings() const {
    return data().canConvert<BundleSettingsQsp>();
  }


  /**
   * Returns true if a BundleSolutionInfo is stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If a BundleSolutionInfo is stored in the data of the item or not.
   */
  bool ProjectItem::isBundleSolutionInfo() const {
    return data().canConvert<BundleSolutionInfo *>();
  }


  /**
   * Returns true if an Image is stored in the data of the item. Returns false
   * otherwise.
   *
   * @return @b bool If an Image is stored in the data of the item or not.
   */
  bool ProjectItem::isImage() const {
    return data().canConvert<Image *>();
  }


  /**
   * Returns true if an ImageList is stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If an ImageList is stored in the data of the item or not.
   */
  bool ProjectItem::isImageList() const {
    return data().canConvert<ImageList *>();
  }


  /**
   * Returns true if a Control is stored in the data of the item. Returns false
   * otherwise.
   *
   * @return @b bool If a Control is stored in the data of the item or not.
   */
  bool ProjectItem::isControl() const { 
    return data().canConvert<Control *>();
  }


  /**
   * Returns true if a ControlList is stored in the data of the item.
   * Returns false
   * otherwise.
   *
   * @return @b bool If a ControlList is stored in the data of the item or not.
   */
  bool ProjectItem::isControlList() const {
    return data().canConvert<ControlList *>();
  }


  /**
   * Returns true if a CorrelationMatrix is stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If a CorrelationMatrix is stored in the data of the item or not.
   */
  bool ProjectItem::isCorrelationMatrix() const {
    return data().canConvert<CorrelationMatrix>();
  }


  /**
   * Returns true if a Project is stored in the data of the item. Returns false
   * otherwise.
   *
   * @return @b bool If a Project is stored in the data of the item or not.
   */
  bool ProjectItem::isProject() const { 
    return data().canConvert<Project *>();
  }


  /**
   * Returns true if a GuiCameraQsp is stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If a GuiCameraQsp is stored in the item or not.
   */
  bool ProjectItem::isGuiCamera() const {     
    return data().canConvert<GuiCameraQsp>();
  }


  /**
   * Returns true if a TargetBodyQsp is stored in the data of the item.
   * Returns false otherwise.
   *
   * @return @b bool If a TargetBodyQsp is stored in the item or not.
   */
  bool ProjectItem::isTargetBody() const { 
    return data().canConvert<TargetBodyQsp>();
  }


  /**
   * Sets the text, icon, and data to those of another item.
   *
   * @param[in] item (ProjectItem *) The other item.
   */
  void ProjectItem::setProjectItem(ProjectItem *item) {
    setText( item->text() );
    setIcon( item->icon() );
    setData( item->data() );
  }


  /**
   * Sets the text, icon, and data corresponding to BundleResults.
   *
   * @param[in] bundleResults (BundleResults) The BundleResults.
   */
  void ProjectItem::setBundleResults(BundleResults bundleResults) {
    setText("Results");
    setIcon( QIcon(":results") );
    setData( QVariant::fromValue<BundleResults>(bundleResults) );
  }


  /**
   * Sets the text, icon, and data corresponding to BundleSettings.
   *
   * @param[in] bundleSettings (BundleSettingsQsp) The BundleSettingsQsp.
   */
  void ProjectItem::setBundleSettings(BundleSettingsQsp bundleSettings) {
    setText("Settings");
    setIcon( QIcon(":settings") );
    setData( QVariant::fromValue<BundleSettingsQsp>(bundleSettings) );
  }


  /**
   * Sets the text, icon, and data corresponding to a BundleSolutionInfo.
   *
   * @param[in] bundleSolutionInfo (BundleSolutionInfo *) The BundleSolutionInfo.
   */
  void ProjectItem::setBundleSolutionInfo(BundleSolutionInfo *bundleSolutionInfo) {
    setText( bundleSolutionInfo->runTime() );
    setIcon( QIcon(":results") );
    setData( QVariant::fromValue<BundleSolutionInfo *>(bundleSolutionInfo) );
  }


  /**
   * Sets the text, icon, and data corresponding to an Image.
   *
   * @param[in] image (Image *) The Image.
   */
  void ProjectItem::setImage(Image *image) {
    setText( QFileInfo( image->fileName() ).fileName() );
    setIcon( QIcon(":pictures") );
    setData( QVariant::fromValue<Image *>(image) );
  }


  /**
   * Sets the text, icon, and data corresponding to an ImageList.
   *
   * @param[in] imageList (ImageList *) The ImageList.
   */
  void ProjectItem::setImageList(ImageList *imageList) {
    setText( imageList->name() );
    setIcon( QIcon(":pictures") );
    setData( QVariant::fromValue<ImageList *>(imageList) );
  }


  /**
   * Sets the text, icon, and data corresponding to a list of ImageList.
   */
  void ProjectItem::setImages() {
    setText("Images");
    setIcon( QIcon(":pictures") );
    setData( QVariant() );
  }


  /**
   * Sets the text, icon, and data corresponding to a Control.
   *
   * @param[in] control (Control *) The Control.
   */
  void ProjectItem::setControl(Control *control) {
    setText( QFileInfo( control->fileName() ).fileName() );
    setIcon( QIcon(":pointReg") );
    setData( QVariant::fromValue<Control *>(control) );
  }


  /**
   * Sets the text, icon, and data corresponding to a ControlList.
   *
   * @param[in] controlList (ControlList *) The ControlList.
   */
  void ProjectItem::setControlList(ControlList *controlList) {
    setText( controlList->name() );
    setIcon( QIcon(":folder") );
    setData( QVariant::fromValue<ControlList *>(controlList) );
  }


  /**
   * Sets the text, icon, and data corresponding to a list of ControlList.
   */
  void ProjectItem::setControls() {
    setText("Control Networks");
    setIcon( QIcon(":layers") );
    setData( QVariant() );
  }

  
  /**
   * Sets the text, icon, and data corresponding to a CorrelationMatrix.
   *
   * @param[in] correlationMatrix (CorrelationMatrix) The CorrelationMatrix.
   */
  void ProjectItem::setCorrelationMatrix(CorrelationMatrix correlationMatrix) {
    setText("Correlation Matrix");
    setIcon( QIcon(":pointReg") );
    setData( QVariant::fromValue<CorrelationMatrix>(correlationMatrix) );
  }


  /**
   * Sets the text, icon, and data corresponding to a Project.
   *
   * @param[in] project (Project *) The Project.
   */
  void ProjectItem::setProject(Project *project) {
    setText( project->name() );
    setIcon( QIcon(":data") );
    setData( QVariant::fromValue<Project *>(project) );
  }


  /**
   * Sets the text, icon, and data corresponding to a list of Results.
   */
  void ProjectItem::setResults() {
    setText("Results");
    setIcon( QIcon(":results") );
    setData( QVariant() );
  }


  /**
   * Sets the text, icon, and data corresponding to a GuiCameraQsp.
   *
   * @param[in] guiCamera (GuiCameraQsp) The GuiCameraQsp.
   */
  void ProjectItem::setGuiCamera(GuiCameraQsp guiCamera) {
    setText( guiCamera->displayProperties()->displayName() );
    setIcon( QIcon(":camera") );
    setData( QVariant::fromValue<GuiCameraQsp>(guiCamera) );
  }


  /**
   * Sets the text, icon, and data corresponding to a GuiCameraQsp.
   */
  void ProjectItem::setGuiCameraList() {
    setText("Sensors");
    setIcon( QIcon(":camera") );
    setData( QVariant() );
  }
  
  
  /**
   * Sets the text, icon, and data corresponding to ShapeModels.
   */
  void ProjectItem::setShapeModels() {
    setText("Shape Models");
    setIcon( QIcon(":dem") );
    setData( QVariant() );
  }
  
  
  /**
   * Sets the text, icon, and data corresponding to SpaceCraft.
   */
  void ProjectItem::setSpacecraft() {
    setText("Spacecraft");
    setIcon( QIcon(":spacecraft") );
    setData( QVariant() );
  }
  

  /**
   * Sets the text, icon, and data corresponding to a TargetBodyQsp.
   *
   * @param targetBody Target body to set data from
   */
  void ProjectItem::setTargetBody(TargetBodyQsp targetBody) {
    setText( targetBody->displayProperties()->displayName() );
    if (targetBody->displayProperties()->displayName() == "MOON")
      setIcon( QIcon(":moon") );
    else if (targetBody->displayProperties()->displayName() == "Enceladus")
      setIcon( QIcon(":enceladus") );
    else if (targetBody->displayProperties()->displayName() == "Mars")
      setIcon( QIcon(":mars") );
    else if (targetBody->displayProperties()->displayName() == "Titan")
      setIcon( QIcon(":titan") );
    else
      setIcon( QIcon(":moonPhase") );
    setData( QVariant::fromValue<TargetBodyQsp>(targetBody) );
  }
  

  /**
   * Sets the text, icon, and data corresponding to a TargetBodyList.
   */
  void ProjectItem::setTargetBodyList() {
    setText("Target Body");
    setIcon( QIcon(":moonPhase") );
    setData( QVariant() );
  }


  /**
   * Finds and returns the first item in the model that contains the
   * data in the role.
   *
   * @param[in] value (const QVariant &) The data.
   * @param[in] role (int) The role.
   *
   * @return @b ProjectItem* The found item.
   */
  ProjectItem *ProjectItem::findItemData(const QVariant &value, int role) {
    if ( data(role) == value ) {
      return 0;
    }
    
    for (int i=0; i<rowCount(); i++) {
      ProjectItem *item = child(i)->findItemData(value, role);
      if (item) {
        return item;
      }
    }

    return 0;
  }


  /**
   * Appends an item to the children of this item.
   *
   * @param[in] item (ProjectItem *) The item to append.
   */
  void ProjectItem::appendRow(ProjectItem *item) {
    QStandardItem::appendRow(item);
  }

 
  /**
   * Returns the child item at a given row.
   *
   * @param[in] row (int) The row.
   *
   * @return @b ProjectItem* The child item.
   */
  ProjectItem *ProjectItem::child(int row) const {
    return static_cast<ProjectItem *>( QStandardItem::child(row) );
  }


  /**
   * Inserts an item to the children of this item at the row.
   *
   * @param[in] row (int) The row.
   * @param[in] item (ProjectItem *) The item.
   */
  void ProjectItem::insertRow(int row, ProjectItem *item) {
    QStandardItem::insertRow(row, item);
  }


  /**
   * Returns the ProjectItemModel associated with this item.
   *
   * @return @b ProjectItemModel* The model.
   */
  ProjectItemModel *ProjectItem::model() const {
    return static_cast<ProjectItemModel *>( QStandardItem::model() );
  }


  /**
   * Returns the parent item of this item.
   *
   * @return @b ProjectItem* The parent item.
   */
  ProjectItem *ProjectItem::parent() const {
    return static_cast<ProjectItem *>( QStandardItem::parent() );
  }


  /**
   * Sets the child at the given row to an item.
   *
   * @param[in] row (int) The row.
   * @param[in] item (ProjectItem *) The item.
   */
  void ProjectItem::setChild(int row, ProjectItem *item) {
    QStandardItem::setChild(row, item);
  }


  /**
   * Removes the child item at the given row and returns the removed item.
   *
   * @param[in] row (int) The row.
   *
   * @return @b ProjectItem* The item.
   */
  ProjectItem *ProjectItem::takeChild(int row) {
    QList<QStandardItem *> items = QStandardItem::takeRow(row);
    
    if ( items.isEmpty() ) {
      return 0;
    }

    return static_cast<ProjectItem *>( items.first() );
  }
  
}
