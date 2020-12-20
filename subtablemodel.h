#pragma once

#include <QAbstractTableModel>

/*!
 * \class SubtableModel
 * \brief Табличная модель
 *
 * Модель для очень больших таблиц. Модель использует подтаблицы, для
 * корректного отображения при больших данных.
 * Модель с возможностью объединения столбцов и с объединением ячеек.
 * Для подключения в QML необходимо написать import cpp.SubtableModel 12.34.
 * Для использования модели в cpp коде, необходимо унаследоваться от этого
 * класса и переопределить функции totalRowCount и totalColumnCount.
 */
class SubtableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int subtableOrientation READ subtableOrientation WRITE setSubtableOrientation NOTIFY subtableOrientationChanged)
    Q_PROPERTY(int subtableSizeMax READ subtableSizeMax WRITE setSubtableSizeMax NOTIFY subtableSizeMaxChanged)
    Q_PROPERTY(int subtableCount READ subtableCount NOTIFY subtableCountChanged)
public:
    static const QString ITEM_NAME;     // SubtableModel
    static const bool IS_QML_REG;
    static const int SUBTABLE_ORIENTATION_DEFAULT = Qt::Vertical;
    static const int SUBTABLE_SIZE_MAX_DEFAULT = -1;
    explicit SubtableModel(QObject *parent = nullptr);
    virtual ~SubtableModel() = default;

    enum SubtableRole {
        SubtableRoleBEGIN = Qt::UserRole,
        // alias std roles
        SubtableRoleDisplay = SubtableRoleBEGIN, // текст (DisplayRole)
        SubtableRoleAlignment,          // выравнивание текста (TextAlignmentRole)
        SubtableRoleBackground,         // цвет фона (BackgroundRole)
        SubtableRoleToolTip,            // всплывающая подсказка (ToolTipPropertyRole)
        // just for Cell
        SubtableRoleReadOnly,           // только на чтение
        SubtableRoleEnabled,            // активность ячейки
        SubtableRoleSpanH,              // объединение по горизонтали
        SubtableRoleSpanV,              // объединение по вертикали
        SubtableRoleValidator,          // валидатор изменений
        SubtableRoleDropdown,           // выпадающие подсказки при редактировании
        // just for Header
        SubtableRoleWidth,              // ширина
        SubtableRoleHeight,             // высота
        SubtableRoleResized,            // возможность менять размеры
        SubtableRolePin,                // закреплена ли строка/столбец
        SubtableRoleGroup,              // являится ли сворачиваемым?
        SubtableRoleDeploy,             // развернута ли группа
        SubtableRoleGroupSize,          // размер группы
        SubtableRoleIndexInGroup,       // индекс в группе

        SubtableRoleEND
    };
    Q_ENUM(SubtableRole)
    static const std::array<QString, SubtableRoleEND - SubtableRoleBEGIN> SUBTABLE_ROLE_STR;

    /*!
     * \brief Возвращает количество строк для полной таблицы
     *
     * Заменяет стандартную функцию rowCount(). Функцию rowCount()
     * переопределять нлеьзя!
     * \return количество строк.
     */
    Q_INVOKABLE virtual int totalRowCount() const = 0;

    /*!
     * \brief Возвращает количество столбцов для полной таблицы
     *
     * Заменяет стандартную функцию columnCount(). Функцию columnCount()
     * переопределять нельзя!
     * \return количество столбцов.
     */
    Q_INVOKABLE virtual int totalColumnCount() const = 0;

    /*!
     * \brief Возвращает количество строк в ПОДТАБЛИЦАХ
     *
     * Не перегружать функцию!
     * \see totalRowCount.
     * \param parent - родитель, не используется.
     * \return количество строк в подтаблицах.
     */
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Возвращает количество столбцов в ПОДТАБЛИЦАХ
     *
     * Не перегружать функцию!
     * \see totalColumnCount.
     * \param parent - родитель, не используется.
     * \return количество строк в подтаблицах.
     */
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /*!
     * \brief Функция получения данных
     *
     * Вызывает стандартную функцию data.
     * \param subtable - номер подтаблицы.
     * \param row - номер строки.
     * \param column - номер колонки.
     * \param role - роль.
     * \return данные ячейки.
     */
    Q_INVOKABLE QVariant subtableData(int subtable, int row, int column, int role = Qt::DisplayRole) const;

    /*!
     * \brief Функция изменения данных
     *
     * Вызывает стандартную функцию setData.
     * \param subtable - номер подтаблицы.
     * \param row - номер строки.
     * \param column - номер колонки.
     * \param value - измененные данные.
     * \param role - роль.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool subtableSetData(int subtable, int row, int column, const QVariant &value, int role = Qt::EditRole);

    /*!
     * \brief Получает данные заголовка
     *
     * Вызывает стандартную функцию headerData.
     * \param subtable - номер подтаблицы.
     * \param section - индекс в подтаблице.
     * \param orientation - оринтация заголовка.
     * \param role - роль.
     * \return данные заголовка.
     */
    Q_INVOKABLE QVariant subtableHeaderData(int subtable, int section, Qt::Orientation orientation, int role) const;

    /*!
     * \brief Возвращает код роли
     * \param role - строковое представление роли.
     * \return код роли.
     */
    Q_INVOKABLE int getStrRole(QByteArray role) const;

    /*!
     * \brief Орентация создания подтаблиц
     * \return Qt::Orientation.
     */
    Q_INVOKABLE int subtableOrientation() const { return _subTableOrientation; }

    /*!
     * \brief Установка орентации создания подтаблиц
     * \param orientation - Qt::Orientation.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool setSubtableOrientation(int orientation);

    /*!
     * \brief Максимальный размер в одной подтаблице
     *
     * Размер по направлению subtableOrientation() будут создавать
     * дополнительные подтаблицы при необходимости.
     * \return максимальное количество строк.
     */
    Q_INVOKABLE int subtableSizeMax() const;

    /*!
     * \brief Установка максимального размера подтаблицы
     * \param sizeMax - максимальный размер.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool setSubtableSizeMax(int sizeMax);

    /*!
     * \brief Возвращает количество подтаблиц
     * \return количество подтаблиц.
     */
    Q_INVOKABLE int subtableCount() const;

    /*!
     * \brief Возвращает абсолютный индекс строки
     * \param row - индекс в подтаблице.
     * \param subtable - индекс подтаблицы.
     * \return абсолютный индекс строки.
     */
    Q_INVOKABLE int absoluteRow(int row, int subtable) const;

    /*!
     * \brief Возвращает абсолютный индекс столбца
     * \param column - индекс в подтаблице.
     * \param subtable - индекс подтаблицы.
     * \return абсолютный индекс столбца.
     */
    Q_INVOKABLE int absoluteColumn(int column, int subtable) const;

signals:
    void subtableOrientationChanged();
    void subtableSizeMaxChanged();
    void subtableCountChanged();

protected slots:
    /*!
     * \brief Ф-ция проверки изменения количества подтаблиц
     *
     * Необходимо вызвать после реализации rowCount/columnCount, например в
     * конструкторе дочернего класса.
     */
    void checkSubtableCountChanged();

protected:
    bool isIndexValid(int subtable, int row, int column) const;
    bool isSubtableValid(int subtable) const;
    bool isRowValid(int row, int subtable) const;
    bool isColumnValid(int column, int subtable) const;

    /*!
     * \brief Возвращает нужную роль или -1
     * \param role - на проверку.
     * \return -1, если системная неиспользуемая роль, иначе role.
     */
    int isGoodRole(int role) const;
    mutable QHash<int, QByteArray> _rolesId;
    mutable QHash<QByteArray, int> _rolesStr;
    int _subTableOrientation = SUBTABLE_ORIENTATION_DEFAULT;
    mutable int _subTableSizeMax = SUBTABLE_SIZE_MAX_DEFAULT;
    int __subTableCountPrev = 1;  ///< по умолчанию одна таблица
public:
    QHash<int, QByteArray> roleNames() const override;
};

