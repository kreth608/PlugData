/*
 // Copyright (c) 2021-2022 Timothy Schoen
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#pragma once

#include <JuceHeader.h>

#include "Box.h"
#include "Pd/PdPatch.h"
#include "PluginProcessor.h"

extern JUCEApplicationBase* juce_CreateApplication();

class SuggestionComponent;
struct GraphArea;
class Edge;
class PlugDataPluginEditor;
class PlugDataPluginProcessor;
class Canvas : public Component, public Value::Listener,  public LassoSource<Component*>
{
   public:
    Canvas(PlugDataPluginEditor& parent, const pd::Patch& patch, bool isGraph = false, bool isGraphChild = false);

    ~Canvas() override;

    PlugDataPluginEditor& main;
    PlugDataAudioProcessor* pd;

    void paintOverChildren(Graphics&) override;

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;

    void synchronise(bool updatePosition = true);

    bool keyPressed(const KeyPress& key) override;

    void copySelection();
    void removeSelection();
    void pasteSelection();
    void duplicateSelection();
    
    void valueChanged(Value& v) override;

    void checkBounds();

    void paint(Graphics& g) override;
    
    void focusGained(FocusChangeType cause) override;

    void undo();
    void redo();
    
    
    // Multi-dragger functions
    void deselectAll();
    
    void setSelected(Component* component, bool shouldNowBeSelected);
    bool isSelected(Component* component) const;

    void handleMouseDown(Component* component, const MouseEvent& e);
    void handleMouseUp(Component* component, const MouseEvent& e);
    void handleMouseDrag(const MouseEvent& e);

    SelectedItemSet<Component*>& getLassoSelection() override;

    void removeSelectedComponent(Component* component);
    void findLassoItemsInArea(Array<Component*>& itemsFound, const Rectangle<int>& area) override;

    void findDrawables(Graphics& g);
    Box* getSingleSelection();
    
    void showSuggestions(Box* box, TextEditor* editor);
    void hideSuggestions();
    

    Viewport* viewport = nullptr;

    bool connectingWithDrag = false;
    SafePointer<Edge> connectingEdge;

    pd::Patch patch;

    OwnedArray<Box> boxes;
    OwnedArray<Connection> connections;
    
    Value locked;
    Value connectionStyle;

    bool isGraph = false;
    bool isGraphChild = false;

    

    Point<int> zeroPosition = {0, 0};
    Point<int> lastMousePos;

    GraphArea* graphArea = nullptr;
    SuggestionComponent* suggestor = nullptr;

   private:
    SafePointer<TabbedComponent> tabbar;

    LassoComponent<Component*> lasso;
    PopupMenu popupMenu;
    
    
    // Multi-dragger variables
    
    const int minimumMovementToStartDrag = 10;

    bool didStartDragging{false};

    Point<int> mouseDownWithinTarget;
    Point<int> totalDragDelta;

    SelectedItemSet<Component*> selectedComponents;

    Component* componentBeingDragged{nullptr};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Canvas)
};

