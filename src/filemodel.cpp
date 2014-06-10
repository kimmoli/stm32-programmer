#include "filemodel.h"

#include <QDateTime>
#include <QDebug>

#include <QImageReader>

#include <QStandardPaths>

#include <QTimer>

#include <QDebug>

Filemodel::Filemodel(QObject *parent) :
    QAbstractListModel(parent)
{
    _roles[NameRole] = "name";
    _roles[BaseRole] = "base";
    _roles[PathRole] = "path";
    _roles[SizeRole] = "size";
    _roles[TimestampRole] = "time";
    _roles[ExtensionRole] = "ext";
    _roles[DirRole] = "dir";
    _sorting = true;
    _path = QDir::homePath();
    _filter = QStringList() << "*.*";
}

Filemodel::~Filemodel()
{
    _modelData.clear();
}

int Filemodel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _modelData.count();
}

QVariant Filemodel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "get" << QString::number(index.row()) << _roles[role];
    int row = index.row();
    if (row < 0 || row >= _modelData.size())
        return QVariant();
    //return _modelData.at(index.row()).value(_roles.value(role));
    return _modelData[index.row()][_roles[role]];
}

QStringList &Filemodel::getFilter()
{
    return _filter;
}

QString &Filemodel::getPath()
{
    return _path;
}

void Filemodel::setFilter(const QStringList &filter)
{
    //qDebug() << "set filter:" << filter;
    _filter = filter;
}

bool Filemodel::getSorting()
{
    return _sorting;
}

void Filemodel::setSorting(bool newSorting)
{
    _sorting = newSorting;
}

void Filemodel::processPath(const QString &path)
{
    _path = path;
    if (_path == "home")
        _path = QDir::homePath();
    clear();

    //qDebug() << "Processing" << _path << _filter;

    QDir dir(_path);
    const QFileInfoList &list = dir.entryInfoList(_filter, QDir::AllDirs | QDir::NoDot | QDir::NoSymLinks | QDir::Files, (_sorting ? QDir::Time : QDir::Name) | QDir::DirsFirst);
    foreach (const QFileInfo &info, list)
    {
        if (info.isDir())//dir.isRoot() && info.fileName() == "..")
            continue;

        //qDebug() << "adding" << info.absoluteFilePath();

        QVariantMap fileInfo;

        beginInsertRows(QModelIndex(), _modelData.size(), _modelData.size());
        fileInfo["name"] = info.fileName();
        fileInfo["base"] = info.baseName();
        fileInfo["path"] = info.absoluteFilePath();
        fileInfo["size"] = info.size();
        fileInfo["time"] = info.created().toTime_t();
        fileInfo["ext"] = info.suffix();
        fileInfo["dir"] = info.isDir();

        _modelData.append(fileInfo);
        endInsertRows();
        Q_EMIT countChanged();
    }
}

void Filemodel::clear()
{
    beginResetModel();
    _modelData.clear();
    endResetModel();
}

int Filemodel::count()
{
    return _modelData.size();
}

bool Filemodel::remove(int index)
{
    if (index > -1 && index < _modelData.size()) {
        QFile file(_modelData[index]["path"].toString());
        if (file.exists()) {
            beginRemoveRows(QModelIndex(), index, index);
            _modelData.remove(index);
            endRemoveRows();
            Q_EMIT countChanged();
            return file.remove();
        }
        else
            return false;
    }
    else
        return false;
}

QVariantMap Filemodel::get(int index)
{
    if (index > -1 && index < _modelData.size())
        return _modelData[index];
    return QVariantMap();
}

void Filemodel::folderDataReceived(const QVariantList &data)
{
    beginInsertRows(QModelIndex(), _modelData.size(), _modelData.size() + data.size() - 1);

    foreach (const QVariant &fileData, data) {
        _modelData.append(fileData.toMap());
    }

    endInsertRows();

    Q_EMIT countChanged();
}
