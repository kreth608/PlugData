/*
 // Copyright (c) 2021-2022 Timothy Schoen
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */
#include <JuceHeader.h>

#include "Dialogs.h"

#include "PluginEditor.h"
#include "PluginProcessor.h"

#include "SaveDialog.h"
#include "ArrayDialog.h"
#include "SettingsDialog.h"
#include "TextEditorDialog.h"
#include "Canvas.h"

Component* Dialogs::showTextEditorDialog(String text, String filename, std::function<void(String, bool)> callback)
{
    auto* editor = new TextEditorDialog(filename);
    editor->editor.setText(text);
    editor->onClose = std::move(callback);
    return editor;
}

void Dialogs::showSaveDialog(std::unique_ptr<Dialog>* target, Component* centre, String filename, std::function<void(int)> callback)
{
    if (*target)
        return;

    auto* dialog = new Dialog(target, centre, 400, 130, 160, false);
    auto* saveDialog = new SaveDialog(centre, dialog, filename);

    dialog->setViewedComponent(saveDialog);
    saveDialog->cb = std::move(callback);
    target->reset(dialog);
}
void Dialogs::showArrayDialog(std::unique_ptr<Dialog>* target, Component* centre, std::function<void(int, String, String)> callback)
{
    if (*target)
        return;

    auto* dialog = new Dialog(target, centre, 300, 180, 200, false);
    auto* arrayDialog = new ArrayDialog(centre, dialog);
    dialog->setViewedComponent(arrayDialog);
    arrayDialog->cb = std::move(callback);
    target->reset(dialog);
}

void Dialogs::createSettingsDialog(std::unique_ptr<Dialog>* target, AudioProcessor& processor, AudioDeviceManager* manager, ValueTree const& settingsTree)
{
    if (*target)
        return;

    auto* editor = processor.getActiveEditor();
    auto* dialog = new Dialog(target, editor, 675, 500, editor->getBounds().getCentreY() + 250, true);
    auto* settingsDialog = new SettingsDialog(processor, dialog, manager, settingsTree);
    dialog->setViewedComponent(settingsDialog);
    target->reset(dialog);
}

void Dialogs::showObjectMenu(PlugDataPluginEditor* parent, Component* target)
{
    PopupMenu menu;

    // Custom function because JUCE adds "shortcut:" before some keycommands, which looks terrible!
    auto createCommandItem = [parent](const CommandID commandID, String const& displayName) {
        ApplicationCommandInfo info(*parent->getCommandForID(commandID));
        auto* target = parent->ApplicationCommandManager::getTargetForCommand(commandID, info);

        PopupMenu::Item i;
        i.text = displayName;
        i.itemID = (int)commandID;
        i.commandManager = parent;
        i.isEnabled = target != nullptr && (info.flags & ApplicationCommandInfo::isDisabled) == 0;

        String shortcutKey;

        for (auto& keypress : parent->getKeyMappings()->getKeyPressesAssignedToCommand(commandID)) {
            auto key = keypress.getTextDescriptionWithIcons();

            if (shortcutKey.isNotEmpty())
                shortcutKey << ", ";

            shortcutKey << key;
        }

        i.shortcutKeyDescription = shortcutKey.trim();

        return i;
    };

    menu.addItem(createCommandItem(CommandIDs::NewObject, "Empty Object"));
    menu.addSeparator();
    menu.addItem(createCommandItem(CommandIDs::NewNumbox, "Number"));
    menu.addItem(createCommandItem(CommandIDs::NewMessage, "Message"));
    menu.addItem(createCommandItem(CommandIDs::NewBang, "Bang"));
    menu.addItem(createCommandItem(CommandIDs::NewToggle, "Toggle"));
    menu.addItem(createCommandItem(CommandIDs::NewButton, "Button"));
    menu.addItem(createCommandItem(CommandIDs::NewVerticalSlider, "Vertical Slider"));
    menu.addItem(createCommandItem(CommandIDs::NewHorizontalSlider, "Horizontal Slider"));
    menu.addItem(createCommandItem(CommandIDs::NewVerticalRadio, "Vertical Radio"));
    menu.addItem(createCommandItem(CommandIDs::NewHorizontalRadio, "Horizontal Radio"));

    menu.addSeparator();

    menu.addItem(createCommandItem(CommandIDs::NewFloatAtom, "Float Atom"));
    menu.addItem(createCommandItem(CommandIDs::NewSymbolAtom, "Symbol Atom"));
    menu.addItem(createCommandItem(CommandIDs::NewListAtom, "List Atom"));
    menu.addSeparator();

    menu.addItem(createCommandItem(CommandIDs::NewArray, "Array"));
    menu.addItem(createCommandItem(CommandIDs::NewGraphOnParent, "GraphOnParent"));
    menu.addItem(createCommandItem(CommandIDs::NewComment, "Comment"));
    menu.addItem(createCommandItem(CommandIDs::NewCanvas, "Canvas"));
    menu.addSeparator();
    menu.addItem(createCommandItem(CommandIDs::NewKeyboard, "Keyboard"));
    menu.addItem(createCommandItem(CommandIDs::NewVUMeterObject, "VU Meter"));
    menu.addItem(createCommandItem(CommandIDs::NewNumboxTilde, "Signal Numbox"));
    menu.addItem(createCommandItem(CommandIDs::NewOscilloscope, "Oscilloscope"));
    menu.addItem(createCommandItem(CommandIDs::NewFunction, "Function"));

    menu.showMenuAsync(PopupMenu::Options().withMinimumWidth(100).withMaximumNumColumns(1).withTargetComponent(target).withParentComponent(parent),
        [parent](int result) {
            if (result != 0) {
                if (auto* cnv = parent->getCurrentCanvas()) {
                    cnv->attachNextObjectToMouse = true;
                }
            }
        });
}

StringArray DekenInterface::getExternalPaths()
{
    StringArray searchPaths;
    
    for(auto package : PackageManager::getInstance()->packageState) {
        if(!package.hasProperty("AddToPath") || !static_cast<bool>(package.getProperty("AddToPath"))) {
            continue;
        }
        
        searchPaths.add(package.getProperty("Path"));
    }
    
    return searchPaths;
}
