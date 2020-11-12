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
 */
class ExcelModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int subTableOrientation READ subTableOrientation WRITE setSubTableOrientation NOTIFY subTableOrientationChanged)
    Q_PROPERTY(int subTableSizeMax READ subTableSizeMax WRITE setSubTableSizeMax NOTIFY subTableSizeMaxChanged)
    Q_PROPERTY(int subTableCnt READ subTableCnt NOTIFY subTableCntChanged)
public:
    static const int SUBTABLE_ORIENTATION_DEFAULT = Qt::Vertical;
    static const int SUBTABLE_SIZE_MAX_DEFAULT = 5000;
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

        ExcelRoleCNT
    };
    Q_ENUM(ExcelRole)
    static const std::array<QString, ExcelRoleCNT> EXCEL_ROLE_STR;

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
     * \param value
     * \param role - роль.
     * \return true, если удачно.
     */
    Q_INVOKABLE bool excelSetData(int subtable, int row, int column, const QVariant &value, int role = Qt::EditRole);

    /*!
     * \brief Орентация создания подтаблиц
     * \return Qt::Orientation.
     */
    int subTableOrientation() const { return _subTableOrientation; }

    /*!
     * \brief Установка орентации создания подтаблиц
     * \param orientation - Qt::Orientation.
     * \return true, если удачно.
     */
    bool setSubTableOrientation(int orientation);

    /*!
     * \brief Максимальный размер в одной подтаблице
     *
     * Размер по направлению subTableOrientation() будут создавать
     * дополнительные подтаблицы при необходимости.
     * \return максимальное количество строк.
     */
    int subTableSizeMax() const { return _subTableSizeMax; }

    /*!
     * \brief Установка максимального размера подтаблицы
     * \param sizeMax - максимальный размер.
     * \return true, если удачно.
     */
    bool setSubTableSizeMax(int sizeMax);

    /*!
     * \brief Возвращает количество подтаблиц
     * \return количество подтаблиц.
     */
    int subTableCnt() const;

    /*!
     * \brief Возвращает абсолютный индекс строки
     * \param row - индекс в подтаблице.
     * \param subtable - индекс подтаблицы.
     * \return абсолютный индекс строки.
     */
    int absoluteRow(int row, int subtable) const;

    /*!
     * \brief Возвращает абсолютный индекс столбца
     * \param column - индекс в подтаблице.
     * \param subtable - индекс подтаблицы.
     * \return абсолютный индекс столбца.
     */
    int absoluteColumn(int column, int subtable) const;

signals:
    void subTableOrientationChanged();
    void subTableSizeMaxChanged();
    void subTableCntChanged();

protected slots:
    /*!
     * \brief Ф-ция проверки изменения количества подтаблиц
     *
     * Необходимо вызвать после реализации rowCount/columnCount, например в
     * конструкторе дочернего класса.
     */
    void checkSubTableCntChanged();

protected:
    bool isIndexValid(int subtable, int row, int column) const;
    bool isGoodSubTable(int subtable) const;
    bool isGoodRow(int row, int subtable) const;
    bool isGoodColumn(int column, int subtable) const;
    /*!
     * \brief Возвращает нужную роль или -1
     * \param role - на проверку.
     * \return -1, если системная неиспользуемая роль, иначе число
     * роль которого больше или равно Qt::UserRole.
     */
    int isGoodRole(int role) const;
    mutable QHash<int, QByteArray> _rolesId;
    int _subTableOrientation = SUBTABLE_ORIENTATION_DEFAULT;
    int _subTableSizeMax = SUBTABLE_SIZE_MAX_DEFAULT;
    int __subTableCntPrev = 0;  ///< следит за испусканием subTableCntChanged()
public:
    QHash<int, QByteArray> roleNames() const override;
};

