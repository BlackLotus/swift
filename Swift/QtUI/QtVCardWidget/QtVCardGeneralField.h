/*
 * Copyright (c) 2012 Tobias Markmann
 * Licensed under the simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "QtCloseButton.h"
#include "QtTagComboBox.h"

namespace Swift {

/*
 *	covers features like:
 *		- preffered (star ceckbox)
 *		- combo check boxh
 *		- label
 *		- remove button
 */
class QtVCardGeneralField : public QWidget {
		Q_OBJECT
		Q_PROPERTY(bool editable READ isEditable WRITE setEditable NOTIFY editableChanged)
		Q_PROPERTY(bool empty READ isEmpty)

	public:
		explicit QtVCardGeneralField(QWidget* parent = 0, QGridLayout* layout = 0, bool editable = false, int row = 0, QString label = QString(),
										bool preferrable = true, bool taggable = true);
		virtual ~QtVCardGeneralField();

		void initialize();

		virtual bool isEditable() const;
		virtual void setEditable(bool);

		virtual bool isEmpty() const = 0;

		void setPreferred(const bool preferred);
		bool getPreferred() const;

	protected:
		virtual void setupContentWidgets() = 0;
		virtual void customCleanup();

		QtTagComboBox* getTagComboBox() const;
		QGridLayout* getGridLayout() const;

	signals:
		void editableChanged(bool);
		void deleteField(QtVCardGeneralField*);
		
	public slots:
		void handleCloseButtonClicked();

	protected:
		QList<QWidget*> childWidgets;

	private:
		bool editable;
		bool preferrable;
		bool taggable;
		QGridLayout* layout;
		int row;
		QCheckBox* preferredCheckBox;
		QLabel* label;
		QString labelText;
		QtTagComboBox* tagComboBox;
		QLabel* tagLabel;
		QtCloseButton* closeButton;
};

}