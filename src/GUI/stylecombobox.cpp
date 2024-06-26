#include <QPen>
#include <QPainter>
#include <QResizeEvent>
#include "common/util.h"
#include "stylecombobox.h"


#define MIN_LINE_LENGTH  60
#define LINE_WIDTH_RATIO 7

static Qt::PenStyle styles[] = {Qt::SolidLine, Qt::DashLine, Qt::DotLine,
  Qt::DashDotLine, Qt::DashDotDotLine};

QIcon StyleComboBox::icon(Qt::PenStyle style)
{
	qreal ratio(devicePixelRatioF());
	QSize size(iconSize());
	QPixmap pm(size * ratio);
	pm.setDevicePixelRatio(ratio);
	pm.fill(Qt::transparent);

	QBrush brush(QPalette().brush(QPalette::Active, QPalette::WindowText));
	QPen pen(brush, size.height() / LINE_WIDTH_RATIO, style);

	QPainter painter(&pm);
	painter.setPen(pen);
	painter.drawLine(0, size.height() / 2, size.width(), size.height() / 2);

	return QIcon(pm);
}

StyleComboBox::StyleComboBox(QWidget *parent) : QComboBox(parent)
{
	QSize is = iconSize();
	setIconSize(QSize(MIN_LINE_LENGTH, is.height()));

	for (size_t i = 0; i < ARRAY_SIZE(styles); i++)
		addItem(icon(styles[i]), QString(), QVariant((int)styles[i]));
}

void StyleComboBox::setValue(Qt::PenStyle value)
{
	for (int i = 0; i < count(); i++) {
		if ((Qt::PenStyle) itemData(i).toInt() == value) {
			setCurrentIndex(i);
			return;
		}
	}
}

void StyleComboBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::PaletteChange)
		for (int i = 0; i < count(); i++)
			setItemIcon(i, icon(styles[i]));

	QComboBox::changeEvent(e);
}
