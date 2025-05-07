#include "SpinBoxDelegate.h"
#include <qspinbox.h>
//一个可以调控数值的控件
SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}

QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	auto* editor = new QSpinBox(parent);
	editor->setMaximum(100);
	editor->setMinimum(0);
	return editor;

}

void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	int value = index.model()->data(index, Qt::EditRole).toInt();
	auto* spinbox = static_cast<QSpinBox*>(editor);
	spinbox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	auto* spinbox = static_cast<QSpinBox*>(editor);
	spinbox->interpretText();
	int value = spinbox->value();
	model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

SpinBoxDelegate::~SpinBoxDelegate()
{}
