#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <QString>
#include <functional>

// Forward declarations
class RedisManager;

/**
 * @brief Workflow namespace for managing base Redis examples workflows
 */
namespace Workflow {

/**
 * @brief Available workflow types for base examples
 */
enum class WorkflowType {
    String,         ///< String operations demo
    Hash,           ///< Hash operations demo
    List,           ///< List operations demo
    Set,            ///< Set operations demo
    SortedSet,      ///< Sorted set operations demo
    Expiration,     ///< Expiration operations demo
    UserProfile,    ///< User profile example
    Leaderboard,    ///< Game leaderboard example
    All,            ///< Run all demos
    Help,           ///< Show help information
    None            ///< No workflow selected
};

/**
 * @brief Execute a workflow based on the given argument
 * @param arg The workflow argument (can be name, number, or special string)
 * @return 0 on success, non-zero on failure
 */
int executeWorkflow(const QString& arg);

/**
 * @brief Execute a specific workflow type
 * @param type The workflow type to execute
 * @return 0 on success, non-zero on failure
 */
int executeWorkflow(WorkflowType type);

/**
 * @brief Print the interactive menu
 */
void printMenu();

/**
 * @brief Print help information
 */
void printHelp();

/**
 * @brief Run the interactive menu and get user choice
 * @return The workflow type selected by the user
 */
WorkflowType runInteractiveMenu();

/**
 * @brief Parse a string argument to a workflow type
 * @param arg The string argument to parse
 * @return The corresponding workflow type
 */
WorkflowType parseWorkflowType(const QString& arg);

/**
 * @brief Get the name of a workflow type
 * @param type The workflow type
 * @return The workflow type name as string
 */
QString getWorkflowName(WorkflowType type);

} // namespace Workflow

#endif // WORKFLOW_H
