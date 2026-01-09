#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <QString>
#include <functional>

// Forward declarations
class ImageManager;

/**
 * @brief Workflow namespace for managing image management workflows
 */
namespace Workflow {

/**
 * @brief Available workflow types
 */
enum class WorkflowType {
    Upload,     ///< Upload images
    Retrieve,   ///< Retrieve images
    Metadata,   ///< Manage image metadata
    Search,     ///< Search images
    Complete,   ///< Complete workflow (upload, retrieve, update, search, delete)
    All,        ///< Run all workflows
    Help,       ///< Show help information
    None        ///< No workflow selected
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
