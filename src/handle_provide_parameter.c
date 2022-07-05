#include "boilerplate_plugin.h"

// EDIT THIS: Remove this function and write your own handlers!
// static void handle_swap_exact_eth_for_tokens(ethPluginProvideParameter_t *msg, context_t *context) {
//     if (context->go_to_offset) {
//         if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
//             return;
//         }
//         context->go_to_offset = false;
//     }
//     switch (context->next_param) {
//         case MIN_AMOUNT_RECEIVED:  // amountOutMin
//             copy_parameter(context->amount_received,
//                            msg->parameter,
//                            sizeof(context->amount_received));
//             context->next_param = PATH_OFFSET;
//             break;
//         case PATH_OFFSET:  // path
//             context->offset = U2BE(msg->parameter, PARAMETER_LENGTH - 2);
//             context->next_param = BENEFICIARY;
//             break;
//         case BENEFICIARY:  // to
//             copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
//             context->next_param = PATH_LENGTH;
//             context->go_to_offset = true;
//             break;
//         case PATH_LENGTH:
//             context->offset = msg->parameterOffset - SELECTOR_SIZE + PARAMETER_LENGTH * 2;
//             context->go_to_offset = true;
//             context->next_param = TOKEN_RECEIVED;
//             break;
//         case TOKEN_RECEIVED:  // path[1] -> contract address of token received
//             copy_address(context->token_received, msg->parameter, sizeof(context->token_received));
//             context->next_param = UNEXPECTED_PARAMETER;
//             break;
//         // Keep this
//         default:
//             PRINTF("Param not supported: %d\n", context->next_param);
//             msg->result = ETH_PLUGIN_RESULT_ERROR;
//             break;
//     }
// }

static void handle_delegate(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case DELEGATEE:
            copy_address(context->delegatee, msg->parameter, sizeof(context->delegatee));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}


static void handle_reactivate_channel(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case CHANNEL_FEES_AMOUNT:
            copy_parameter(context->channel_fees_amount, msg->parameter, sizeof(context->channel_fees_amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this 
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_deactivate_channel(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case TOKENS_RECEIVED:
            copy_parameter(context->tokens, msg->parameter, sizeof(context->tokens));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this 
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_create_channel(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }

    switch (context->next_param) {
        case CHANNEL_TYPE:
            context->next_param = CHANNEL_DATA;
            break;

        case CHANNEL_DATA:
            context->next_param = CHANNEL_CREATION_FEES_AMOUNT;
            break;

        case CHANNEL_CREATION_FEES_AMOUNT:
            copy_parameter(context->channel_fees_amount, msg->parameter, sizeof(context->channel_fees_amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_approve_dai(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case BENEFICIARY:
            copy_address(context->beneficiary, msg->parameter, sizeof(context->beneficiary));
            context->next_param = DAI_AMOUNT;
            break;

        case DAI_AMOUNT:
            copy_parameter(context->channel_fees_amount, msg->parameter, sizeof(context->channel_fees_amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;

        // Keep this 
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}


void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case EPNS_ADD_DELEGATE:
        case EPNS_REMOVE_DELEGATE:
            handle_delegate(msg, context);
            break;

        case EPNS_REACTIVATE_CHANNEL:
            handle_reactivate_channel(msg, context);
            break;
        
        case EPNS_DEACTIVATE_CHANNEL:
            handle_deactivate_channel(msg, context);
            break;

        case EPNS_CREATE_CHANNEL:
            handle_create_channel(msg, context);
            break;

        case EPNS_DAI_APPROVE:
            handle_approve_dai(msg, context);
            break;
        
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}