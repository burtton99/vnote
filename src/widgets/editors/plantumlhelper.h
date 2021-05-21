#ifndef PLANTUMLHELPER_H
#define PLANTUMLHELPER_H

#include <QObject>

#include <QStringList>
#include <QPair>

#include <core/noncopyable.h>

namespace vnotex
{
    class PlantUmlHelper : private Noncopyable
    {
    public:
        static PlantUmlHelper &getInst()
        {
            static PlantUmlHelper inst;
            return inst;
        }

        static QPair<bool, QString> testPlantUml(const QString &p_plantUmlJarFile);

    private:
        PlantUmlHelper();

        void init(const QString &p_plantUmlJarFile,
                  const QString &p_graphvizFile,
                  const QString &p_overriddenCommand);

        static void prepareProgramAndArgs(const QString &p_plantUmlJarFile,
                                          const QString &p_graphvizFile,
                                          QString &p_program,
                                          QStringList &p_args);

        static QStringList getArgsToUse(const QStringList &p_args);

        QString m_program;

        QStringList m_args;

        // If this is not empty, @m_program and @m_args will be ignored.
        QString m_overriddenCommand;
    };
}

#endif // PLANTUMLHELPER_H
