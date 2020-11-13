#pragma once

#include <QAbstractTableModel>

/*!
 * \brief Табличная модель
 *
 * Модель с возможностью объединения столбцов и с объединением ячеек.
 * Для подключения в QML необходимо написать import ExcelModel 12.34.
 * Для использования модели в cpp коде, необходимо унаследоваться от этого
 * класса.
 * Так же, для отображения больших таблиц, используется подтаблицы и
 * функции excelData / excelSetData.
 * \todo Переименовать в SubtableModel.
 */
class ExcelModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int subTableOrientation READ subTableOrientation WRITE setSubTableOrientation NOTIFY subTableOrientationChanged)
    Q_PROPERTY(int subTableSizeMax READ subTableSizeMax WRITE setSubTableSizeMax NOTIFY subTableSizeMaxChanged)
    Q_PROPERTY(int subTableCount READ subTableCount NOTIFY subTableCountChanged)
public:
    static const int SUBTABLE_ORIENTATION_DEFAULT = Qt::Vertical;
    static const int SUBTABLE_SIZE_MAX_DEFAULT = -1;
    explicit ExcelModel(QObject *parent = nullptr);
    virtual ~ExcelModel() = default;

    enum ExcelRole {
        // alias std roles
        ExcelRoleDisplay,           // текст (DisplayRole)
        ExcelRoleAlignment,         // выравнивание текста (TextAlignmentRole)
        ExcelRoleBackground,        // цвет фона (BackgroundRole)
        ExcelRoleToolTip,           // всплывающая подсказка (ToolTipPropertyRole)
        // just for Cell
        ExcelRoleReadOnly,          // только на чтение
        ExcelRoleEnabled,           // активность ячейки
        ExcelRoleSpanH,             // объединение по горизонтали
        ExcelRoleSpanV,             // объединение по вертикали
        ExcelRoleValidator,         // валидатор изменений
        ExcelRoleDropdown,          // выпадающие подсказки при редактировании
        // выбранные ячейки возможно не нужно делать ролями.
//        ExcelRoleSelected,          // выбранная ячейка (может быть только одна в модели)
//        ExcelRoleInSelectedArea,    // выбранная область
        // just for Header
        ExcelRoleWidth,             // ширина
        ExcelRoleHeight,            // высота
        ExcelRoleResized,           // возможность менять размеры
        ExcelRoleGroup,             // номер группы
        ExcelRoleIndexInGroup,      // индекс в группе
        ExcelRoleDeploy,            // развернута ли группа

        ExcelRoleCOUNT
    };
    Q_ENUM(ExcelRole)
    static const std::array<QString, ExcelRoleCOUNT> EXCEL_ROLE_STR;

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
    Q_INVOKABLE QVariant excelData(int subtable, int row, int column, int role = Qt::DisplayRole) const;

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
    Q_INVOKABLE bool excelSetData(int subtable, int row, int column, const QVariant &value, int role = Qt::EditRole);

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
    Q_INVOKABLE QVariant excelHeaderData(int subtable, int section, Qt::Orientation orientation, int role) const;

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
    Q_INVOKABLE int subTableOrientation() const { return _subTableOrientation; }

    /*!
     * \brief Установка орентации создания подтаблиц
     * \param orientation - Qt::Orientation.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool setSubTableOrientation(int orientation);

    /*!
     * \brief Максимальный размер в одной подтаблице
     *
     * Размер по направлению subTableOrientation() будут создавать
     * дополнительные подтаблицы при необходимости.
     * \return максимальное количество строк.
     */
    Q_INVOKABLE int subTableSizeMax() const;

    /*!
     * \brief Установка максимального размера подтаблицы
     * \param sizeMax - максимальный размер.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool setSubTableSizeMax(int sizeMax);

    /*!
     * \brief Возвращает количество подтаблиц
     * \return количество подтаблиц.
     */
    Q_INVOKABLE int subTableCount() const;

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
    void subTableOrientationChanged();
    void subTableSizeMaxChanged();
    void subTableCountChanged();

protected slots:
    /*!
     * \brief Ф-ция проверки изменения количества подтаблиц
     *
     * Необходимо вызвать после реализации rowCount/columnCount, например в
     * конструкторе дочернего класса.
     */
    void checkSubTableCountChanged();

protected:
    bool isIndexValid(int subtable, int row, int column) const;
    bool isSubTableValid(int subtable) const;
    bool isRowValid(int row, int subtable) const;
    bool isColumnValid(int column, int subtable) const;
    /*!
     * \brief Возвращает нужную роль или -1
     * \param role - на проверку.
     * \return -1, если системная неиспользуемая роль, иначе число
     * роль которого больше или равно Qt::UserRole.
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

