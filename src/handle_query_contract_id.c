#include "boilerplate_plugin.h"

// Sets the first screen to display.
// void handle_query_contract_id(void *parameters) {
//     ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
//     const context_t *context = (const context_t *) msg->pluginContext;
//     // msg->name will be the upper sentence displayed on the screen.
//     // msg->version will be the lower sentence displayed on the screen.

//     // For the first screen, display the plugin name.
//     strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

//     // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
//     if (context->selectorIndex == SWAP_EXACT_ETH_FOR_TOKENS) {
//         strlcpy(msg->version, "Swap", msg->versionLength);
//         msg->result = ETH_PLUGIN_RESULT_OK;
//     } else {
//         PRINTF("Selector index: %d not supported\n", context->selectorIndex);
//         msg->result = ETH_PLUGIN_RESULT_ERROR;
//     }
// }


// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

    // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
    if (context->selectorIndex == EPNS_ADD_DELEGATE) {
        strlcpy(msg->version, "Add Delegate", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else  if (context->selectorIndex == EPNS_REMOVE_DELEGATE) {
        strlcpy(msg->version, "Remove Delegate", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else  if (context->selectorIndex == EPNS_REACTIVATE_CHANNEL) {
        strlcpy(msg->version, "ReActivate Channel", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else  if (context->selectorIndex == EPNS_DEACTIVATE_CHANNEL) {
        strlcpy(msg->version, "DeActivate Channel", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else  if (context->selectorIndex == EPNS_CREATE_CHANNEL) {
        strlcpy(msg->version, "Create Channel", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else  if (context->selectorIndex == EPNS_DAI_APPROVE) {
        strlcpy(msg->version, "Approve DAI", msg->versionLength);
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Selector index: %d not supported\n", context->selectorIndex);
        msg->result = ETH_PLUGIN_RESULT_ERROR;
    }
}