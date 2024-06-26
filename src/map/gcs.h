#ifndef GCS_H
#define GCS_H

#include "common/kv.h"
#include "datum.h"
#include "angularunits.h"
#include "primemeridian.h"

class GCS
{
public:
	GCS() {}
	GCS(const Datum &datum, const PrimeMeridian &primeMeridian,
	  const AngularUnits &angularUnits) : _datum(datum),
	  _primeMeridian(primeMeridian), _angularUnits(angularUnits) {}

	bool operator==(const GCS &other) const
	{
		return (_datum == other._datum && _primeMeridian == other._primeMeridian
		  && _angularUnits == other._angularUnits);
	}

	const PrimeMeridian &primeMeridian() const {return _primeMeridian;}
	const AngularUnits &angularUnits() const {return _angularUnits;}
	const Datum &datum() const {return _datum;}

	bool isNull() const {return _datum.isNull() && _angularUnits.isNull()
	  && _primeMeridian.isNull();}
	bool isValid() const {return _datum.isValid() && _angularUnits.isValid()
	  && _primeMeridian.isValid();}

	Coordinates toWGS84(const Coordinates &c) const
	{
		return datum().toWGS84(Coordinates(_primeMeridian.toGreenwich(c.lon()),
		  c.lat()));
	}
	Coordinates fromWGS84(const Coordinates &c) const
	{
		Coordinates ds(datum().fromWGS84(c));
		return Coordinates(_primeMeridian.fromGreenwich(ds.lon()), ds.lat());
	}

	static GCS gcs(int id);
	static GCS gcs(int geodeticDatum, int primeMeridian, int angularUnits);
	static GCS gcs(const QString &name);
	static const GCS &WGS84();

	static bool loadList(const QString &path);
	static QList<KV<int, QString> > list();
	static QList<KV<int, QString> > WGS84List();

private:
	class Entry;

	static QList<Entry> defaults();

	Datum _datum;
	PrimeMeridian _primeMeridian;
	AngularUnits _angularUnits;

	static QList<Entry> _gcss;
};

#ifndef QT_NO_DEBUG
QDebug operator<<(QDebug dbg, const GCS &gcs);
#endif // QT_NO_DEBUG

#endif // GCS_H
